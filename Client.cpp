#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include "Client_grpc.h"
#include <time.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    std::string address = "localhost:50051";
    BankClient cli(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
    std::cout << std::endl;

    if (argc != 2)
    {
        printf("Must enter number of iterations\n");
        return 0;
    }
    int nbAccounts = atoi(argv[1]);

    FILE *filewithnames = fopen("tenkusernames.txt", "r");
    // FILE *filewithnames = fopen("names.txt", "r");

    char line[256];

    char *names[nbAccounts];

    //  ########### Get Names  #############

    printf("Get %d names from file\n", nbAccounts);

    for (int i = 0; i < nbAccounts; i++)
    {
        char *lines = fgets(line, sizeof(line), filewithnames);
        char *name_copy = (char *)malloc(strlen((char *)lines));
        if (!name_copy)
            return -1;
        strncpy(name_copy, (char *)lines, strlen((char *)lines) + 1);
        int len = strlen(name_copy);
        if (name_copy[len - 1] == '\n')
            name_copy[len - 1] = 0;
        names[i] = (char *)malloc(strlen((char *)name_copy) + 1);
        names[i] = name_copy;
    }
    fclose(filewithnames);

    struct timespec start_time, end_time, tmp_time, current_time, start_lat, end_lat;
    long long int elapsed_ns;

    //  ########### Create Accounts  #############

    printf("\nCreate %d accounts from names\n", nbAccounts);

    int sizeThroughput = 100;
    FILE *fileThroughputcrea = fopen("throughputcrea.csv", "w");
    fprintf(fileThroughputcrea, "i,throughputcrea,time\n");
    float *throughputcrea = (float *)malloc((nbAccounts / sizeThroughput) * sizeof(float));
    FILE *filelatencycrea = fopen("latencycrea.csv", "w");
    fprintf(filelatencycrea, "i,latencycrea,time\n");
    float *latencycrea = (float *)malloc(nbAccounts * sizeof(float));

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for (int i = 0; i < nbAccounts; i++)
    {
        latencycrea[i] = cli.CreateAccount(names[i]);
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        elapsed_ns = (current_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (current_time.tv_nsec - start_time.tv_nsec);
        fprintf(filelatencycrea, "%d,%f,%f\n", i, latencycrea[i] * pow(10, 6), elapsed_ns / pow(10, 9));
        if (i % sizeThroughput == 0 && i != 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &current_time);
            elapsed_ns = (current_time.tv_sec - tmp_time.tv_sec) * pow(10, 9) + (current_time.tv_nsec - tmp_time.tv_nsec);
            throughputcrea[i / sizeThroughput] = sizeThroughput / (float)(elapsed_ns / pow(10, 6));
            elapsed_ns = (current_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (current_time.tv_nsec - start_time.tv_nsec);
            fprintf(fileThroughputcrea, "%d,%f,%f\n", i, throughputcrea[i / sizeThroughput], elapsed_ns / pow(10, 9));
            clock_gettime(CLOCK_MONOTONIC, &tmp_time);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (end_time.tv_nsec - start_time.tv_nsec);
    float creation_time = (float)(elapsed_ns / pow(10, 9));
    fclose(filelatencycrea);
    fclose(fileThroughputcrea);

    //  ########### Operation Add & Sub Amount #############

    FILE *fileThroughputope = fopen("throughputope.csv", "w");
    fprintf(fileThroughputope, "i,throughputope,time\n");
    float *throughputope = (float *)malloc((nbAccounts / sizeThroughput) * sizeof(float));
    FILE *fileLatencyope = fopen("latencyope.csv", "w");
    fprintf(fileLatencyope, "i,latencyope,time\n");
    float *latencyope = (float *)malloc(nbAccounts * sizeof(float));
    srand(time(NULL));
    int nbope = nbAccounts;
    // int nbope = 10;
    int count = 0;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    clock_gettime(CLOCK_MONOTONIC, &tmp_time);

    printf("\nMake %d operations in the different accounts from file\n", nbope);

    for (int i = 0; i < nbope; i++)
    {
        int randomAmount = rand() % 10000;
        int randomAccountDebit = rand() % nbAccounts;
        int randomAccountCredit = rand() % nbAccounts;
        clock_gettime(CLOCK_MONOTONIC, &start_lat);
        cli.SubAmount(names[randomAccountDebit], randomAmount);
        cli.AddAmount(names[randomAccountCredit], randomAmount);
        clock_gettime(CLOCK_MONOTONIC, &end_lat);
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        latencyope[i] = ((end_lat.tv_sec - start_lat.tv_sec) * pow(10, 9) + (end_lat.tv_nsec - start_lat.tv_nsec)) / pow(10, 9);
        elapsed_ns = (current_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (current_time.tv_nsec - start_time.tv_nsec);
        fprintf(fileLatencyope, "%d,%f,%f\n", i, latencyope[i] * pow(10, 6), elapsed_ns / pow(10, 9));
        // computing and storing throughputope
        if (i % sizeThroughput == 0 && i != 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &current_time);
            elapsed_ns = (current_time.tv_sec - tmp_time.tv_sec) * pow(10, 9) + (current_time.tv_nsec - tmp_time.tv_nsec);
            throughputope[i / sizeThroughput] = sizeThroughput / (float)(elapsed_ns / pow(10, 6));
            elapsed_ns = (current_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (current_time.tv_nsec - start_time.tv_nsec);
            fprintf(fileThroughputope, "%d,%f,%f\n", i, throughputope[i / sizeThroughput], elapsed_ns / pow(10, 9));
            clock_gettime(CLOCK_MONOTONIC, &tmp_time);
        }
        count++;
        if (count % 200 == 0)
        {
            printf(" %d ", count);
        }
        if (count % 2000 == 0)
        {
            printf(" \n");
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    elapsed_ns = (end_time.tv_sec - start_time.tv_sec) * pow(10, 9) + (end_time.tv_nsec - start_time.tv_nsec);
    float ope_time = (float)(elapsed_ns / pow(10, 9));
    fclose(fileThroughputope);
    fclose(fileLatencyope);

#if DEBUG
    cli.ListAccount();
#endif
    // *******************  STATS  ******************************
    printf("*************       STATS         *******************\n");

    printf("%d accounts = %.2f secs\n", nbAccounts, creation_time);
    printf("%d operations = %.2f secs\n", nbope, ope_time);

    float avg_latency_crea = creation_time / nbAccounts; // ns
    float avg_throughput_crea = (nbAccounts / creation_time) * pow(10, -3);
    float avg_latency_ope = ope_time / nbope;
    float avg_throughput_ope = (nbAccounts / ope_time) * pow(10, -3);

    float variance, st_deviation, sum, ecart = 0;
    char *namemeasure[4] = {(char *)"latency_crea", (char *)"throughput_crea", (char *)"latency_ope", (char *)"throughput_ope"};
    float *value[4]{latencycrea, throughputcrea, latencyope, throughputope};
    float avg[4] = {avg_latency_crea, avg_throughput_crea, avg_latency_ope, avg_throughput_ope};
    float puiss[2] = {(float)pow(10, 6), (float)1};
    char *units[4] = {(char *)"µs", (char *)"account/ms", (char *)"µs", (char *)"ope/ms"};
    int number[4] = {nbAccounts, nbAccounts / sizeThroughput, nbope, nbope / sizeThroughput};

    for (int k = 0; k < 4; k++)
    {
        sum = 0;
        int nb = number[k];
        for (int i = 0; i < nb; i++)
        {
            ecart = value[k][i] - avg[k];
            sum = sum + pow(ecart, 2);
        }
        variance = sum / (float)nb;
        st_deviation = sqrt(variance);
        printf("\n");
        printf("mean %s = %.2f %s\n", namemeasure[k], avg[k] * puiss[k % 2], units[k]);
        printf("std of %s = %.2f\n", namemeasure[k], st_deviation * puiss[k % 2]);
    }

    // cli.CreateAccount("Eloi");
    // cli.AddAmount("Eloi", 100);
    // cli.CreateAccount("ouaoua");
    // cli.AddAmount("ouaoua", 800);
    // cli.CreateAccount("ptit ben");
    // cli.AddAmount("ptit ben", -200);
    // cli.SubAmount("ptit ben", 93200);

    // cli.ListAccount();
}
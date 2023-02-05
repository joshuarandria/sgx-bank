import pandas as pd
import matplotlib.pyplot as plt

df_num = [0, 1, 2, 3]
df_name = ['latencycrea', 'throughputcrea', 'latencyope', 'throughputope']
df_unite = ['µs', 'ope/ms']
# df_list = [0, 1, 2, 3]
df_legend = ['LatencyCrea', 'ThroughputCrea',
             'LatencyOpe', 'ThroughputOpe']
df_groupsize = [200, 4, 150, 3]
mean_interv = 200

for n in df_num:
    ######  GET DATAFRAME AND FORMAT #########
    df = pd.read_csv(df_name[n]+'.csv')
    colname = df_name[n]
    df.drop(df[df['i'] == 0].index, inplace=True)
    # df_list[n] = df

    ##############  STATS  ###################
    dfstat = df[df_name[n]]  # lat en ms et throughput en ope/ms
    print(df_name[n])
    str_mean = 'mean = ' + format(
        dfstat.mean(), '.0f') + ' ' + df_unite[n % 2]+'\n'
    str_std = 'std = ' + format(df[colname].std(), '.1f')+'\n'
    print(str_mean + str_std)

    ##############  GRAPH  ####################
    groupsize = df_groupsize[n]
    colmean = 'mean_by_'+str(groupsize)
    df[colmean] = None

    # Iterate over the DataFrame, grouped by groupsize rows
    for i, group in enumerate(df.groupby(df.index//groupsize)):
        start = group[0]*groupsize
        end = start+groupsize
        df.loc[start:end-1, colmean] = group[1][colname].mean()

    # Convert 'time' column to datetime format
    df['time'] = pd.to_datetime(df['time']*pow(10, 9))

    time_interval = pd.Timedelta(milliseconds=mean_interv)
    slices = df.groupby(df['time'].dt.floor(time_interval))

    df['time'] = df['time'].values.astype(int) / 10**9

    # Create the figure and axes
    fig, ax = plt.subplots()

    for i, (_, slice_df) in enumerate(slices):
        # Plot the slice on the same axes
        ax.plot(df['time'], df[colmean], color='blue', label="_time")
        mean = slice_df[colmean].mean()
        x_mean = slice_df['time'].iloc[0], slice_df['time'].iloc[-1]
        y_mean = [mean, mean]
        ax.plot(x_mean, y_mean, '-', color='red', label="_xmean_ymean")

    plt.text(.99, .98, str_mean, ha='right',
             va='top', transform=ax.transAxes)
    plt.text(.99, .94, str_std, ha='right',
             va='top', transform=ax.transAxes)
    plt.xlabel('Time (s)')
    plt.ylabel(df_legend[n]+' ('+df_unite[n % 2]+')')
    plt.ylim(0, max(df[colmean]*1.4))
    plt.title(df_legend[n])

plt.show()

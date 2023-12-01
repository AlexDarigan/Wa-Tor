# Chart Generation Code
import plotly
import plotly.graph_objects as go

maximum = []
minimum = []
averages = []

font=dict(
    family="Courier New, monospace",
    size=18,  # Set the font size here
    color="RebeccaPurple"
)

idx = 0
threads = [1, 2, 4, 8]
for file in ["single.txt", "two.txt", "four.txt", "eight.txt"]:
    with open(file, "r") as f:
        durations = list(map(lambda string: int(string), f.read().split(",")[:-1]))
        generations = list(range(0, len(durations)))
        maximum.append(max(durations))
        minimum.append(min(durations))
        averages.append(sum(durations) / len(durations))
        fig = go.Figure(data=go.Scatter(x=generations, y=durations, mode='lines'))
        fig.update_layout(title=f"{threads[idx]} Thread(s) over 1000 Generations", xaxis_title="Generations", yaxis_title="Milliseconds", font=font)
        fig.show()
        fig.write_image(f"With{threads[idx]}Threads.svg")
    idx = idx + 1

fig = go.Figure(data=go.Bar(x=["1 Thread", "2 Threads", "4 Threads", "8 Threads"], y=maximum))
fig.update_layout(title="Max Milliseconds per Thread", yaxis_title="Max Milliseconds", xaxis_title="Thread Count", font=font)
fig.write_image("MaxTimePerThread.svg")

fig = go.Figure(data=go.Bar(x=["1 Thread", "2 Threads", "4 Threads", "8 Threads"], y=minimum))
fig.update_layout(title="Min Milliseconds per Thread", yaxis_title="Min Milliseconds", xaxis_title="Thread Count", font=font)
fig.write_image("MinTimePerThread.svg")

fig = go.Figure(data=go.Bar(x=["1 Thread", "2 Threads", "4 Threads", "8 Threads"], y=averages))
fig.update_layout(title="Average Milliseconds per Thread", yaxis_title="Average Milliseconds", xaxis_title="Thread Count", font=font)
fig.write_image("AverageTimePerThread.svg")

fig = go.Figure(data=go.Scatter(x=["1 Thread", "2 Threads", "4 Threads", "8 Threads"], y=maximum, mode='lines', name="Max Time"))
fig.add_scatter(x=["1 Thread", "2 Threads", "4 Threads", "8 Threads"], y=averages, mode='lines', name = "Average TIme")
fig.add_scatter(x=["1 Thread", "2 Threads", "4 Threads", "8 Threads"], y=minimum, mode='lines', name = "Min Time")
fig.update_layout(xaxis_title="Thread Count", yaxis_title="Time in Milliseconds",  font = font)
fig.write_image("AllTimesPerThreadCount.svg")
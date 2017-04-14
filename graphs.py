
import plotly
import plotly.graph_objs as go


def graphFvsDX(commandsX, undesirables, middles, posThreshold, negThreshold):
	fs = [c.f for c in commandsX]
	dxs = [c.dx for c in commandsX]

	undesire = [c.f for c in undesirables]
	mid = [c.f for c in middles]

	trace0 = go.Scatter(
			x = dxs,
			y = fs,
			name = 'Threshold vs DX',
			mode = 'markers',
    		marker = dict(
        		size = 10,
        		color = 'rgba(0, 20, 200, .5)')
		)

	trace1 = go.Scatter(
			x = dxs,
			y = [posThreshold for i in dxs],
			name = 'X pos threshold (Right Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace2 = go.Scatter(
			x = dxs,
			y = [negThreshold for i in dxs],
			name = 'X neg threshold (Left Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace3 = go.Scatter(
			x = dxs,
			y = undesire,
			name = 'Undesirable Points (Conflicting derivitive vs pos)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(200, 20, 0, .5)')
		)

	trace4 = go.Scatter(
			x = dxs,
			y = mid,
			name = 'Middle points (desirable, but no command sent)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(45, 45, 45, .3)')
		)

	data = [trace0, trace1, trace2, trace3, trace4]

	layout = dict(title = 'Threshold vs DX',
		xaxis = dict(title = 'dx/df (Derivative of x with respect to frames)'),
		yaxis = dict(title = 'Threshold Calculation Value'),
		)

	fig = dict(data=data, layout=layout)
	plotly.offline.plot(fig, filename='FvsDX.html')


def graphFvsDY(commandsY, undesirables, middles, posThreshold, negThreshold):
	fs = [c.f for c in commandsY]
	dys = [c.dy for c in commandsY]

	undesire = [c.f for c in undesirables]
	mid = [c.f for c in middles]

	trace0 = go.Scatter(
			x = dys,
			y = fs,
			name = 'Threshold vs DY',
			mode = 'markers',
    		marker = dict(
        		size = 10,
        		color = 'rgba(0, 20, 200, .5)')
		)

	trace1 = go.Scatter(
			x = dys,
			y = [posThreshold for i in dys],
			name = 'Y pos threshold (Up Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace2 = go.Scatter(
			x = dys,
			y = [negThreshold for i in dys],
			name = 'Y neg threshold (Down Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace3 = go.Scatter(
			x = dys,
			y = undesire,
			name = 'Undesirable Points (Conflicting derivitive vs pos)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(200, 20, 0, .5)')
		)

	trace4 = go.Scatter(
			x = dys,
			y = mid,
			name = 'Middle points (desirable, but no command sent)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(45, 45, 45, .3)')
		)

	data = [trace0, trace1, trace2, trace3, trace4]

	layout = dict(title = 'Threshold vs DY',
		xaxis = dict(title = 'dy/df (Derivative of y with respect to frames)'),
		yaxis = dict(title = 'Threshold Calculation Value'),
		)

	fig = dict(data=data, layout=layout)
	plotly.offline.plot(fig, filename='FvsDY.html')


def graphFvsXbias(commandsX, undesirables, middles, posThreshold, negThreshold):
	fs = [c.f for c in commandsX]
	xbs = [c.xBias for c in commandsX]

	undesire = [c.f for c in undesirables]
	mid = [c.f for c in middles]

	trace0 = go.Scatter(
			x = xbs,
			y = fs,
			name = 'Threshold vs XBias',
			mode = 'markers',
    		marker = dict(
        		size = 10,
        		color = 'rgba(0, 20, 200, .5)')
		)

	trace1 = go.Scatter(
			x = xbs,
			y = [posThreshold for i in xbs],
			name = 'X pos threshold (Right Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace2 = go.Scatter(
			x = xbs,
			y = [negThreshold for i in xbs],
			name = 'X neg threshold (Left Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace3 = go.Scatter(
			x = xbs,
			y = undesire,
			name = 'Undesirable Points (Conflicting derivitive vs pos)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(200, 20, 0, .5)')
		)

	trace4 = go.Scatter(
			x = xbs,
			y = mid,
			name = 'Middle points (desirable, but no command sent)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(45, 45, 45, .3)')
		)

	data = [trace0, trace1, trace2, trace3, trace4]

	layout = dict(title = 'Threshold vs XBias',
		xaxis = dict(title = 'XBias component of threshold value'),
		yaxis = dict(title = 'Threshold Calculation Value'),
		)

	fig = dict(data=data, layout=layout)
	plotly.offline.plot(fig, filename='FvsXBias.html')


def graphFvsYbias(commandsY, undesirables, middles, posThreshold, negThreshold):
	fs = [c.f for c in commandsY]
	dys = [c.yBias for c in commandsY]

	undesire = [c.f for c in undesirables]
	mid = [c.f for c in middles]

	trace0 = go.Scatter(
			x = dys,
			y = fs,
			name = 'Threshold vs YBias',
			mode = 'markers',
    		marker = dict(
        		size = 10,
        		color = 'rgba(0, 20, 200, .5)')
		)

	trace1 = go.Scatter(
			x = dys,
			y = [posThreshold for i in dys],
			name = 'Y pos threshold (Up Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace2 = go.Scatter(
			x = dys,
			y = [negThreshold for i in dys],
			name = 'Y neg threshold (Down Command)',
			line = dict(
				color = ('rgb(20, 200, 0)'),
				width = 4)
		)

	trace3 = go.Scatter(
			x = dys,
			y = undesire,
			name = 'Undesirable Points (Conflicting derivitive vs pos)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(200, 20, 0, .5)')
		)

	trace4 = go.Scatter(
			x = dys,
			y = mid,
			name = 'Middle points (desirable, but no command sent)',
			mode = 'markers',
			marker = dict(
				size = 10,
				color = 'rgba(45, 45, 45, .3)')
		)

	data = [trace0, trace1, trace2, trace3, trace4]

	layout = dict(title = 'Threshold vs YBias',
		xaxis = dict(title = 'YBias component of threshold value'),
		yaxis = dict(title = 'Threshold Calculation Value'),
		)

	fig = dict(data=data, layout=layout)
	plotly.offline.plot(fig, filename='FvsYBias.html')


def graphXCommands(commandsList):
	rightn = 0
	leftn = 0

	for c in commandsList:
		if (c.command == 1 and c.f != 0):
			rightn += 1
		elif (c.command == 0 and c.f != 0):
			leftn += 1

	right = go.Bar(
			x = ['Right Commands Sent'],
			y = [rightn],
			name='Right'
		)

	left = go.Bar(
			x = ['Left Commands Sent'],
			y = [leftn],
			name='Left'
		)

	data = [left, right]

	layout = go.Layout(
    	title='X Commands for Threshold Values'
	)

	fig = go.Figure(data=data, layout=layout)

	plotly.offline.plot(fig, filename='xCommands.html')


def graphYCommands(commandsList):
	upn = 0
	downn = 0

	for c in commandsList:
		if (c.command == 1 and c.f != 0):
			upn += 1
		elif (c.command == 0 and c.f != 0):
			downn += 1

	up = go.Bar(
			x = ['Up Commands Sent'],
			y = [upn],
			name='Up'
		)

	down = go.Bar(
			x = ['Down Commands Sent'],
			y = [downn],
			name='Down'
		)

	data = [down, up]

	layout = go.Layout(
    	title='Y Commands for Threshold Values'
	)

	fig = go.Figure(data=data, layout=layout)

	plotly.offline.plot(fig, filename='yCommands.html')


def graphPercentages(commandsX, commandsY, undesirableX, undesirableY, midX, midY):
	validX = len(commandsX)
	undesX = len(undesirableX)
	mX = len(midX)
	validY = len(commandsY)
	undesY = len(undesirableY)
	mY = len(midY)

	trace0 = go.Bar(
			x = ['Valid X Commands'],
			y = [validX],
			name='Valid X'
		)

	trace1 = go.Bar(
			x = ['Undesirable X'],
			y = [undesX],
			name='Undesirable X (Ex. Moving Left, Position is Right)'
		)

	trace2 = go.Bar(
			x = ['Middle X'],
			y = [mX],
			name='Middle X (Between Two Thresholds)'
		)

	trace3 = go.Bar(
			x = ['Valid Y Commands'],
			y = [validY],
			name='Valid Y'
		)

	trace4 = go.Bar(
			x = ['Undesirable Y'],
			y = [undesY],
			name='Undesirable Y (Ex. Moving Up, Position is Down)'
		)

	trace5 = go.Bar(
			x = ['Middle Y'],
			y = [mY],
			name='Middle Y (Between Two Thresholds)'
		)

	data = [trace0, trace3, trace2, trace5, trace1, trace4]

	layout = go.Layout(
    	title='Threshold Value Type Freq.'
	)

	fig = go.Figure(data=data, layout=layout)

	plotly.offline.plot(fig, filename='threshTypeFreq.html')
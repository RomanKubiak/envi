/* Envi javascript */
console.log ("Envi.js")

var rpcConfig = { endPoint: '/jsonrpc', namespace: 'envi' };

function rpcSuccess(rpcResult)
{
	console.log ("rpcSuccess");
	console.log (rpcResult);

	for (key in result.params)
	{
		console.log (key);
		console.log (result.params[key]);
	}
}

function rpcFailure(rpcResult)
{
	console.log ("rpcFailure")
	console.log (rpcResult);
}

$.jsonRPC.setup(rpcConfig);

$.jsonRPC.request('status', { params: [], success: rpcSuccess, error: rpcFailure } );

/* Envi javascript */
console.log ("Envi.js")

var rpcEnviConfig = { endPoint: '/jsonrpc', namespace: 'envi' };
var rpcCoreConfig = { endPoint: '/jsonrpc', namespace: 'core' };

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

$.jsonRPC.setup(rpcCoreConfig);
$.jsonRPC.request('ping', { params: [], success: rpcSuccess, error: rpcFailure } );

module.exports = async function (context, req) {
    context.log('JavaScript HTTP trigger function processed a request.');

    const ewelink = require('ewelink-api');
    const deviceId = req.body.deviceId;
    const email = req.body.email;
    const secret = req.body.secret;
    const region = req.body.region

    /* instantiate class */
    const connection = new ewelink({
        email: email,
        password: secret,
        region: region,
    });

    var result = 'No Status';
    result = await connection.toggleDevice(deviceId);

    context.res = {
        body: result
    };
}
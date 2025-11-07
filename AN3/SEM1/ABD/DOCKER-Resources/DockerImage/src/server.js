const express = require('express');
const app = express()

app.get('/',(req,res)=>{
        res.and("Welcome to my awesome app!");
});

app.listen(3000, function () {
    console.log("app listening on port 3000");
});
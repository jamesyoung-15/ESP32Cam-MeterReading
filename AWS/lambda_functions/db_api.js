const aws = require('aws-sdk');
aws.config.update({
    region: 'us-east-1'
})


const dynamodb = new aws.DynamoDB.DocumentClient();


const dynamodbTableName = 'water-meter-db-restapi' //CHANGE to your db 
const singleDataPath = '/watermeter/data'
const allDataPath = '/watermeter/alldata'

exports.handler = async function(event) {
    console.log('Request event: ', event);
    let response;
    switch(true) {
      case event.httpMethod === 'GET' && event.path === singleDataPath:
        response = await getSingleData(event.queryStringParameters.id);
        break;
      case event.httpMethod === 'GET' && event.path === allDataPath:
        response = await getAllData();
        break;
      case event.httpMethod === 'POST' && event.path === singleDataPath:
        response = await saveData(JSON.parse(event.body));
        break;
      case event.httpMethod === 'DELETE' && event.path === singleDataPath:
        response = await deleteData(JSON.parse(event.body).id);
        break;
      default:
        response = buildResponse(404, '404 Not Found');
    }
    return response;
}


async function getSingleData(dataId){
    const params = {
        TableName: dynamodbTableName,
        Key: {
            'id': dataId
        }
    }
    return await dynamodb.get(params).promise().then((response) => {
        return buildResponse(200, response.Item);
    }, (error) => {
        console.error('Can do custom error handling here. Log: ', error);
    });
}

async function getAllData(){
    const params = {
        TableName: dynamodbTableName
      }
      const allData = await scanDynamoRecords(params, []);
      const body = {
        data: allData
      }
      return buildResponse(200, body);
}

async function scanDynamoRecords(scanParams, itemArray) {
    try {
        const dynamoData = await dynamodb.scan(scanParams).promise();
        itemArray = itemArray.concat(dynamoData.Items);
        if (dynamoData.LastEvaluatedKey) {
            scanParams.ExclusiveStartkey = dynamoData.LastEvaluatedKey;
            return await scanDynamoRecords(scanParams, itemArray);
        }
        return itemArray;
    } 
    catch(error) {
        console.error('Error log:  ', error);
    }
}



async function saveData(requestBody) {
    const params = {
      TableName: dynamodbTableName,
      Item: requestBody
    }
    return await dynamodb.put(params).promise().then(() => {
      const body = {
        Operation: 'SAVE',
        Message: 'SUCCESS',
        Item: requestBody
      }
      return buildResponse(200, body);
    }, (error) => {
      console.error('Error log:  ', error);
    })
}

async function deleteData(id){
  const params = {
    TableName: dynamodbTableName,
    Key: {
      'id': id
    },
    ReturnValues: 'ALL_OLD'
  }
  return await dynamodb.delete(params).promise().then((response) => {
    const body = {
      Operation: 'DELETE',
      Message: 'SUCCESS',
      Item: response
    }
    return buildResponse(200, body);
  }, (error) => {
    console.error('Error log:  ', error);
  })
}

function buildResponse(statusCode, body) {
  return {
    statusCode: statusCode,
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(body)
  }
}
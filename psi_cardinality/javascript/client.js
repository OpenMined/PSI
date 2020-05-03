const cpp_main = require('.');

(async () => {
    // Wait for the initializer to complete loading
    // Upon success, this will immediatly run our C++ main() function
    const { PSICardinality } = await cpp_main(`../../bazel-out/wasm-opt/bin/psi_cardinality/javascript/psi_cardinality_client_${process.env.RUN_DEMO}.js`) // RUN_DEMO = js|wasm

    const numClientElements = 10
    const clientElements = Array.from({length: numClientElements}, (_, i) => `Element ${i}`)
    const clientInputs = PSICardinality.vecFromJSArray(clientElements)

    const client = PSICardinality.PSICardinalityClient.Create()

    console.time('Client Create Request')
    const request = client.CreateRequest(clientInputs)
    console.timeEnd('Client Create Request')
    console.log(`Request: '${request}'`)

    const serverSetup = '{"num_hash_functions":14,"bits":"3epLgv8yYsCYBR5/lJkL68bvaYnKO2ctJDHcLtP1WLYAW/WfBjp/OZt29n8z2aoMfwWbX0XVzzIQuy8FNHOMJRAn6cbf58TW4fkd5LvwZJaHjmb9I2ldP1Tb/0fIT1jICaLaf5pCYnUHP7k96LUMvIJifyLzgIp55xNyORthchjnxNc+JEnuElbmNV60WjOxRYDHimUt2PprtnWuxDOJaFt27POzv8P5CHIh47RLwLlnO863qC5OodVgnM9Iq58YDqYuiPKJ5cy0B/ZVKeJVeRTn0T7TrdwzcEyraD7wKm3uii+T7a9cxgIutdibhx3T"}'
    const serverResponse = '["AjImmhffA84qni8FaVUk0mrHGlPq0euszb8ybGj7Ng8Y","AoFL8Z95cMPn0u2jKK21u2dcvtpxkhLzbj/jOBPRjuYn","AoLlJ4KcrngAxtz6UgLt8XOXAWfD8O/jgLJj7h5GQuUw","ApKzB5L1IYk4TSwU8Lb4OZyLMG9dD9ogfN0OluWHHSNN","ArgHriOim45RG4OwhuFHODuuxyokcFP1WNRwxdqXXnQA","Ar9vWQiEH91kfEgCXJYUvOEFS8rFIny2DS36BmzQP/iI","AyKgOrgBCKU0o08kAV7aWYo3Aua4E+JReEIr7sHgoed+","A4igW3ipQJL7AR6cEsSxtzSwGFvq7O+E8I5YnzXNkoU/","A5ADOJNakGH1bDna5pAXo5oJIgCMDushnWKcvMFVjuUC","A+hbRX2SQBVjIo/ObehKq7Qp3SSf92PPSfMz1YOQoa77"]'

    const intersection = client.ProcessResponse(serverSetup, serverResponse)
    console.log(`intersection: '${intersection}'`)
})();
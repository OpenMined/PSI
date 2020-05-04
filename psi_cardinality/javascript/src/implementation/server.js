/**
 * @implements Server
 */
const ServerImpl = instance => {
  let _instance = instance

  /**
   * @interface Server
   */
  return {
    /**
     * Delete the underlying WASM instance.
     *
     * Should be called before dereferencing this object to prevent the
     * WASM heap from growing indefinitely.
     * @function
     * @name Server#delete
     */
    delete() {
      if (_instance) {
        _instance.delete()
        _instance = null
      }
    },

    /**
     * Creates a setup message from the server's dataset to be sent to the client.
     * The setup message is a JSON-encoded Bloom filter containing H(x)^s for each
     * element x in `inputs`, where s is the server's secret key. The message has
     * the following form:
     *
     *   {
     *     "num_hash_functions": <int>,
     *     "bits": <string>
     *   }
     *
     * `bits` is encoded as Base64.
     * The false-positive rate `fpr` is the probability that any query of size
     * `numClientInputs` will result in a false positive.
     *
     * @typedef {Object} SetupMessage
     * @property {Number} num_hash_functions - The number of hashing functions rounds
     * @property {String} bits - The base64 encoded bits from the underlying bloom filter
     *
     * @function
     * @name Server#CreateSetupMessage
     * @param {Number} fpr False positive rate
     * @param {Number} numClientInputs The number of expected client inputs
     * @param {Array<String>} inputs The server's dataset
     * @returns {SetupMessage} The serialized setup message
     */
    CreateSetupMessage(fpr, numClientInputs, inputs) {
      const { Value, Status } = _instance.CreateSetupMessage(
        fpr,
        numClientInputs,
        inputs
      )
      if (Status) {
        throw new Error(Status.Message)
      }
      return Value
    },

    /**
     * Processes a client query and returns the corresponding server response to
     * be sent to the client. For each encrytped element H(x)^c in the decoded
     * `clientRequest`, computes (H(x)^c)^s = H(X)^(cs) and returns these as a
     * sorted JSON array. Sorting the output prevents the client from matching the
     * individual response elements to the ones in the request, ensuring that they
     * can only learn the intersection size but not individual elements in the
     * intersection.
     *
     * @function
     * @name Server#ProcessRequest
     * @param {String} clientRequest The serialized client request
     * @returns {String} The PSI cardinality
     */
    ProcessRequest(clientRequest) {
      const { Value, Status } = _instance.ProcessRequest(clientRequest)
      if (Status) {
        throw new Error(Status.Message)
      }
      return Value
    },

    /**
     * Returns this instance's private key. This key should only be used to create
     * other server instances. DO NOT SEND THIS KEY TO ANY OTHER PARTY!
     *
     * @function
     * @name Server#GetPrivateKeyBytes
     * @returns {String} A binary string representing the private key
     */
    GetPrivateKeyBytes() {
      return _instance.GetPrivateKeyBytes()
    }
  }
}

export const Server = library => ({
  /**
   * Create a new PSI Cardinality server
   *
   * @function
   * @name Server.CreateWithNewKey
   * @returns {Server} A Server instance
   */
  CreateWithNewKey() {
    const { Value, Status } = library.PSICardinalityServer.CreateWithNewKey()
    if (Status) {
      throw new Error(Status.Message)
    }

    return ServerImpl(Value)
  },
  /**
   * Create a new PSI Cardinality server from a key
   *
   * @function
   * @name Server.CreateFromKey
   * @param {String} key
   * @returns {Server} A Server instance
   */
  CreateFromKey(key) {
    const { Value, Status } = library.PSICardinalityServer.CreateFromKey(key)
    if (Status) {
      throw new Error(Status.Message)
    }

    return ServerImpl(Value)
  }
})

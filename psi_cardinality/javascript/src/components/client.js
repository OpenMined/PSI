export const Client = library => ({ Exception, Vector }) => ({
  /**
   * PSI Cardinality client factory
   *
   * @function
   * @name Client.Create
   * @returns {Client} A Client instance
   */
  Create() {
    let _instance = library.PSICardinalityClient.Create()

    /**
     * @implements Client
     */

    /**
     * @interface Client
     */
    return {
      /**
       * Get the underlying WASM instance
       *
       * @private
       * @readonly
       * @name Client#instance
       * @type {instance}
       */
      get instance() {
        return _instance
      },

      /**
       * Inject this object with a raw WASM instance. No type checking is performed.
       *
       * @private
       * @function
       * @name Client#unsafeInject
       * @param {instance} instance WASM instance
       */
      unsafeInject(instance) {
        if (_instance) {
          _instance.delete()
          _instance = null
        }
        _instance = instance
      },

      /**
       * Delete the underlying WASM instance.
       *
       * Should be called before dereferencing this object to prevent the
       * WASM heap from growing indefinitely.
       * @function
       * @name Client#delete
       */
      delete() {
        if (_instance) {
          _instance.delete()
          _instance = null
        }
      },

      /**
       * Creates a request message to be sent to the server. For each input element
       * x, computes H(x)^c, where c is the secret key of ec_cipher_. The result is
       * sorted to hide the initial ordering of `inputs` and encoded as a JSON
       * array.
       *
       * @function
       * @name Client#CreateRequest
       * @param {Vector<String>} inputs
       * @returns {String} The serialized request
       */
      CreateRequest(inputs) {
        try {
          return _instance.CreateRequest(inputs.instance)
        } catch (e) {
          throw Exception.safe(e)
        }
      },

      /**
       * Processes the server's response and returns the PSI cardinality. The first
       * argument, `setup`, is a bloom filter that encodes encrypted server
       * elements and is sent by the server in a setup phase. The second argument,
       * `response`, is the response received from the server after sending
       * the result of `CreateRequest`.
       *
       * @function
       * @name Client#ProcessResponse
       * @param {String} setup The serialized server setup
       * @param {String} response The serialized server response
       * @returns {Number} The PSI cardinality
       */
      ProcessResponse(setup, response) {
        try {
          return _instance.ProcessResponse(setup, response)
        } catch (e) {
          throw Exception.safe(e)
        }
      }
    }
  }
})

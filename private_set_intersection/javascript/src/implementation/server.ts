import * as psiCardinality from 'psi_'
import { Loader } from '../loader'
import { ERROR_INSTANCE_DELETED } from './constants'

export type Server = {
  readonly delete: () => void
  readonly createSetupMessage: (
    fpr: number,
    numClientInputs: number,
    inputs: readonly string[]
  ) => string
  readonly processRequest: (clientRequest: string) => string
  readonly getPrivateKeyBytes: () => Uint8Array
}

export type ServerWrapper = {
  readonly createWithNewKey: () => Server
  readonly createFromKey: (key: Uint8Array) => Server
}

type ServerWrapperOptions = {
  readonly loader: Loader
}

/**
 * @implements Server
 */
const ServerConstructor = (instance: psiCardinality.Server): Server => {
  let _instance: psiCardinality.Server | null = instance

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
    delete(): void {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      _instance.delete()
      _instance = null
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
     * @name Server#createSetupMessage
     * @param {Number} fpr False positive rate
     * @param {Number} numClientInputs The number of expected client inputs
     * @param {Array<String>} inputs The server's dataset
     * @returns {SetupMessage} The serialized setup message
     */
    createSetupMessage(
      fpr: number,
      numClientInputs: number,
      inputs: readonly string[]
    ): string {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
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
     * @name Server#processRequest
     * @param {String} clientRequest The serialized client request
     * @returns {String} The PSI cardinality
     */
    processRequest(clientRequest: string): string {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
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
     * @name Server#getPrivateKeyBytes
     * @returns {Uint8Array} A binary Uint8Array representing the private key
     */
    getPrivateKeyBytes(): Uint8Array {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      return Uint8Array.from(_instance.GetPrivateKeyBytes())
    }
  }
}

export const ServerWrapperConstructor = ({
  loader
}: ServerWrapperOptions): ServerWrapper => {
  const library: psiCardinality.Library = loader.library

  return {
    /**
     * Create a new PSI Cardinality server
     *
     * @function
     * @name Server.createWithNewKey
     * @returns {Server} A Server instance
     */
    createWithNewKey(): Server {
      const { Value, Status } = library.PsiServer.CreateWithNewKey()
      if (Status) {
        throw new Error(Status.Message)
      }

      return ServerConstructor(Value)
    },
    /**
     * Create a new PSI Cardinality server from a key
     *
     * @function
     * @name Server.createFromKey
     * @param {Uint8Array} key Private key as a binary Uint8Array
     * @returns {Server} A Server instance
     */
    createFromKey(key: Uint8Array): Server {
      const { Value, Status } = library.PsiServer.CreateFromKey(key)
      if (Status) {
        throw new Error(Status.Message)
      }

      return ServerConstructor(Value)
    }
  }
}

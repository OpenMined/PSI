import { CppLibrary, Client } from '../types'
import { Loader } from '../loader'

export type ClientLibrary = {
  readonly Create: () => Promise<Client>
}

type ClientLibraryOptions = {
  readonly Loader: Loader
}

const INSTANCE_DELETED = 'Instance was deleted'

/**
 * @implements Client
 */
const ClientInstanceImpl = (instance: CppLibrary): Client => {
  let _instance: CppLibrary | null = instance

  /**
   * @interface Client
   */
  return {
    /**
     * Delete the underlying WASM instance.
     *
     * Should be called before dereferencing this object to prevent the
     * WASM heap from growing indefinitely.
     * @function
     * @name Client#delete
     */
    delete(): void {
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
     * @param {Array<String>} inputs
     * @returns {String} The serialized request
     */
    CreateRequest(inputs): string {
      if (!_instance) {
        throw new Error(INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.CreateRequest(inputs)
      if (Status) {
        throw new Error(Status.Message)
      }
      return Value
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
    ProcessResponse(setup, response): number {
      if (!_instance) {
        throw new Error(INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.ProcessResponse(setup, response)
      if (Status) {
        throw new Error(Status.Message)
      }
      return Value
    }
  }
}

export const ClientImpl = ({ Loader }: ClientLibraryOptions): ClientLibrary => {
  let library: CppLibrary

  const initialize = async (): Promise<void> => {
    if (!library) {
      const module = await Loader()
      library = module.library
    }
  }

  return {
    /**
     * Create a new PSI Cardinality client
     *
     * @async
     * @function
     * @name Client.Create
     * @returns {Client} A Client instance
     */
    async Create(): Promise<Client> {
      await initialize()
      const { Value, Status } = library.PSICardinalityClient.Create()
      if (Status) {
        throw new Error(Status.Message)
      }
      return ClientInstanceImpl(Value)
    }
  }
}

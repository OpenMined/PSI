import * as psi from 'psi_'
import { Loader } from '../loader'
import { ERROR_INSTANCE_DELETED } from './constants'

export type Client = {
  readonly delete: () => void
  readonly createRequest: (clientInputs: readonly string[]) => string
  readonly processResponse: (
    serverSetup: string,
    serverResponse: string
  ) => number
}

export type ClientWrapper = {
  readonly create: () => Client
}

type ClientWrapperOptions = {
  readonly loader: Loader
}

/**
 * @implements Client
 */
const ClientConstructor = (instance: psi.Client): Client => {
  let _instance: psi.Client | null = instance

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
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      _instance.delete()
      _instance = null
    },

    /**
     * Creates a request message to be sent to the server. For each input element
     * x, computes H(x)^c, where c is the secret key of ec_cipher_. The result is
     * sorted to hide the initial ordering of `inputs` and encoded as a JSON
     * array.
     *
     * @function
     * @name Client#createRequest
     * @param {Array<String>} inputs
     * @returns {String} The serialized request
     */
    createRequest(inputs: readonly string[]): string {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
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
     * @name Client#processResponse
     * @param {String} setup The serialized server setup
     * @param {String} response The serialized server response
     * @returns {Number} The PSI cardinality
     */
    processResponse(setup: string, response: string): number {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.ProcessResponse(setup, response)
      if (Status) {
        throw new Error(Status.Message)
      }
      return Value
    }
  }
}

export const ClientWrapperConstructor = ({
  loader
}: ClientWrapperOptions): ClientWrapper => {
  const library: psi.Library = loader.library

  return {
    /**
     * Create a new PSI client
     *
     * @function
     * @name Client.create
     * @returns {Client} A Client instance
     */
    create(): Client {
      const { Value, Status } = library.PsiClient.Create()
      if (Status) {
        throw new Error(Status.Message)
      }
      return ClientConstructor(Value)
    }
  }
}

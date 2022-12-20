import * as psi from 'psi_'
import { Loader } from '../main/loader'
import { ERROR_INSTANCE_DELETED } from './constants'
import { Request, Response, ServerSetup } from './proto/psi_pb'

export type Client = {
  readonly delete: () => void
  readonly createRequest: (clientInputs: readonly string[]) => Request
  readonly getIntersection: (
    serverSetup: ServerSetup,
    serverResponse: Response
  ) => number[]
  readonly getIntersectionSize: (
    serverSetup: ServerSetup,
    serverResponse: Response
  ) => number
  readonly getPrivateKeyBytes: () => Uint8Array
}

export type ClientWrapper = {
  readonly createWithNewKey: (revealIntersection?: boolean) => Client
  readonly createFromKey: (
    key: Uint8Array,
    revealIntersection?: boolean
  ) => Client
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
     * x, computes H(x)^c, where c is the client's secret key. The result is
     * sorted to hide the initial ordering of `inputs` and returned as a protobuf
     *
     * @function
     * @name Client#createRequest
     * @param {Array<String>} inputs
     * @returns {Request} The Request protobuf
     */
    createRequest(inputs: readonly string[]): Request {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.CreateRequest(inputs)
      if (Status) {
        throw new Error(Status.Message)
      }

      return Request.deserializeBinary(Value)
    },

    /**
     * Processes the server's response and returns the PSI intersection. The first
     * argument, `setup`, is a bloom filter that encodes encrypted server
     * elements and is sent by the server in a setup phase. The second argument,
     * `response`, is the response received from the server after sending
     * the result of `createRequest`.
     *
     * @function
     * @name Client#getIntersection
     * @param {ServerSetup} setup The ServerServer protobuf
     * @param {Response} response The Response protobuf
     * @returns {Number[]} The PSI intersection
     */
    getIntersection(setup: ServerSetup, response: Response): number[] {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }

      const { Value, Status } = _instance.GetIntersection(
        setup.serializeBinary(),
        response.serializeBinary()
      )
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
     * the result of `createRequest`.
     *
     * @function
     * @name Client#getIntersectionSize
     * @param {ServerSetup} setup The ServerServer protobuf
     * @param {Response} response The Response protobuf
     * @returns {Number} The PSI cardinality
     */
    getIntersectionSize(setup: ServerSetup, response: Response): number {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.GetIntersectionSize(
        setup.serializeBinary(),
        response.serializeBinary()
      )
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
     * @name Client#getPrivateKeyBytes
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

export const ClientWrapperConstructor = ({
  loader
}: ClientWrapperOptions): ClientWrapper => {
  const library: psi.Library = loader.library

  return {
    /**
     * Create a new PSI client
     *
     * @function
     * @name Client.createWithNewKey
     * @param {boolean} [revealIntersection=false] - If true, reveals the actual intersection instead of the cardinality.
     * @returns {Client} A Client instance
     */
    createWithNewKey(revealIntersection = false): Client {
      const { Value, Status } =
        library.PsiClient.CreateWithNewKey(revealIntersection)
      if (Status) {
        throw new Error(Status.Message)
      }
      return ClientConstructor(Value)
    },

    /**
     * Create a new PSI client from a key
     *
     * WARNING: This function should be used with caution, since reusing the client key for multiple requests can reveal
     * information about the input sets. If in doubt, use `createWithNewKey`.
     *
     * @function
     * @name Client.createFromKey
     * @param {Uint8Array} key Private key as a binary Uint8Array
     * @param {boolean} [revealIntersection=false] - If true, reveals the actual intersection instead of the cardinality.
     * @returns {Client} A Client instance
     */
    createFromKey(key: Uint8Array, revealIntersection = false): Client {
      const { Value, Status } = library.PsiClient.CreateFromKey(
        key,
        revealIntersection
      )
      if (Status) {
        throw new Error(Status.Message)
      }

      return ClientConstructor(Value)
    }
  }
}

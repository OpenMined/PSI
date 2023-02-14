import * as psi from 'psi_'
import { Loader } from '../main/loader'
import { ERROR_INSTANCE_DELETED } from './constants'
import { Request, Response, ServerSetup } from './proto/psi_pb'

export type Server = {
  readonly delete: () => void
  readonly createSetupMessage: (
    fpr: number,
    numClientInputs: number,
    inputs: readonly string[],
    dataStructure?: psi.DataStructure
  ) => ServerSetup
  readonly processRequest: (clientRequest: Request) => Response
  readonly getPrivateKeyBytes: () => Uint8Array
}

export type ServerWrapper = {
  readonly createWithNewKey: (revealIntersection?: boolean) => Server
  readonly createFromKey: (
    key: Uint8Array,
    revealIntersection?: boolean
  ) => Server
}

type ServerWrapperOptions = {
  readonly loader: Loader
}

/**
 * @implements Server
 */
const ServerConstructor = (
  { DataStructure }: { DataStructure: psi.DataStructure },
  instance: psi.Server
): Server => {
  let _instance: psi.Server | null = instance

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
     * The setup message is a Bloom filter protobuf containing H(x)^s for each
     * element x in `inputs`, where s is the server's secret key.
     *
     * The false-positive rate `fpr` is the probability that any query of size
     * `numClientInputs` will result in a false positive.
     *
     * @function
     * @name Server#createSetupMessage
     * @param {Number} fpr False positive rate
     * @param {Number} numClientInputs The number of expected client inputs
     * @param {Array<String>} inputs The server's dataset
     * @param {DataStructure} [dataStructure=DataStructure.gcs] The server's underlying data structure to use
     * @returns {ServerSetup} The ServerSetup protobuf
     */
    createSetupMessage(
      fpr: number,
      numClientInputs: number,
      inputs: readonly string[],
      dataStructure: psi.DataStructure = DataStructure.GCS
    ): ServerSetup {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.CreateSetupMessage(
        fpr,
        numClientInputs,
        inputs,
        dataStructure
      )
      if (Status) {
        throw new Error(Status.Message)
      }

      return ServerSetup.deserializeBinary(Value)
    },

    /**
     * Processes a client query and returns the corresponding server response to
     * be sent to the client. For each encrytped element H(x)^c in the decoded
     * `clientRequest`, computes (H(x)^c)^s = H(X)^(cs) and returns these as a
     * sorted array. Sorting the output prevents the client from matching the
     * individual response elements to the ones in the request, ensuring that they
     * can only learn the intersection size but not individual elements in the
     * intersection.
     *
     * @function
     * @name Server#processRequest
     * @param {Request} clientRequest The Request protobuf
     * @returns {Response} The Response protobuf
     */
    processRequest(clientRequest: Request): Response {
      if (!_instance) {
        throw new Error(ERROR_INSTANCE_DELETED)
      }
      const { Value, Status } = _instance.ProcessRequest(
        clientRequest.serializeBinary()
      )
      if (Status) {
        throw new Error(Status.Message)
      }
      return Response.deserializeBinary(Value)
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

export type ServerConstructorDependencies = {
  ({ DataStructure }: { DataStructure: psi.DataStructure }): ServerWrapper
}

export const ServerWrapperConstructor =
  ({ loader }: ServerWrapperOptions): ServerConstructorDependencies =>
  ({ DataStructure }): ServerWrapper => {
    const library: psi.Library = loader.library

    return {
      /**
       * Create a new PSI server
       *
       * @function
       * @name Server.createWithNewKey
       * @param {boolean} [revealIntersection=false] - If true, reveals the actual intersection instead of the cardinality.
       * @returns {Server} A Server instance
       */
      createWithNewKey(revealIntersection = false): Server {
        const { Value, Status } =
          library.PsiServer.CreateWithNewKey(revealIntersection)
        if (Status) {
          throw new Error(Status.Message)
        }

        return ServerConstructor({ DataStructure }, Value)
      },
      /**
       * Create a new PSI server from a key
       *
       * @function
       * @name Server.createFromKey
       * @param {Uint8Array} key Private key as a binary Uint8Array
       * @param {boolean} [revealIntersection=false] - If true, reveals the actual intersection instead of the cardinality.
       * @returns {Server} A Server instance
       */
      createFromKey(key: Uint8Array, revealIntersection = false): Server {
        const { Value, Status } = library.PsiServer.CreateFromKey(
          key,
          revealIntersection
        )
        if (Status) {
          throw new Error(Status.Message)
        }

        return ServerConstructor({ DataStructure }, Value)
      }
    }
  }

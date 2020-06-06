import { PackageWrapper } from './package'
import { ClientWrapper } from './client'
import { ServerWrapper } from './server'

export type PSILibrary = {
  readonly package: PackageWrapper
  readonly server?: ServerWrapper
  readonly client?: ClientWrapper
}

type PSIConstructorOptions = {
  readonly packageWrapper: PackageWrapper
  readonly serverWrapper?: ServerWrapper
  readonly clientWrapper?: ClientWrapper
}

/**
 * @implements PSI
 */
export const PSIConstructor = ({
  packageWrapper,
  serverWrapper,
  clientWrapper
}: PSIConstructorOptions): PSILibrary => {
  /**
   * @interface PSI
   */
  return {
    /**
     * @description
     * Information about the package
     *
     * @readonly
     * @name PSI.package
     * @type {Object}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const { version } = psi.package
     */
    ...{ package: packageWrapper },

    /**
     * @description
     * The server side of a Private Set Intersection protocol.
     * See the documentation in PSI.Client for a full description of the
     * protocol.
     *
     * @readonly
     * @name PSI.server
     * @type {Object}
     * @returns {Server}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const server = psi.server.createWithNewKey()
     */
    ...(serverWrapper && { server: serverWrapper }),

    /**
     * @description
     * Client side of a Private Set Intersection protocol. In
     * PSI, two parties (client and server) each hold a dataset, and at
     * the end of the protocol the client learns the size of the intersection of
     * both datasets, while no party learns anything beyond that.
     *
     * This variant of PSI introduces a small false-positive rate (i.e.,
     * the reported cardinality will be slightly larger than the actual cardinality.
     * The false positive rate can be tuned by the server.
     *
     * @readonly
     * @name PSI.client
     * @type {Object}
     * @returns {Client}
     * @example
     * import PSI from '@openmined/psi.js'
     * const psi = await PSI()
     * const client = psi.client.createWithNewKey()
     */
    ...(clientWrapper && { client: clientWrapper })
  }
}

import * as psiCardinality from 'psi_cardinality'
import { Loader } from '../loader'

export type Package = {
  readonly version: string
}

export type PackageWrapper = Package

type PackageWrapperOptions = {
  readonly loader: Loader
}

/**
 * @implements Package
 */
const PackageConstructor = (library: psiCardinality.Library): Package => {
  const Package: psiCardinality.Package = library.Package

  /**
   * @interface Package
   */
  return {
    /**
     * Get the current version of the library.
     *
     * @function
     * @name Package.version
     */
    get version(): string {
      return Package.version()
    }
  }
}

export const PackageWrapperConstructor = ({
  loader
}: PackageWrapperOptions): PackageWrapper => {
  const library: psiCardinality.Library = loader.library
  return PackageConstructor(library)
}

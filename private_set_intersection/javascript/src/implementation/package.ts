import * as psi from 'psi_'
import { Loader } from '../main/loader'

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
const PackageConstructor = (library: psi.Library): Package => {
  const Package: psi.Package = library.Package

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
  const library: psi.Library = loader.library
  return PackageConstructor(library)
}

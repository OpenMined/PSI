import * as psi from 'psi_'
import { Loader } from '../main/loader'

export type DataStructureWrapper = psi.DataStructure

type DataStructureWrapperOptions = {
  readonly loader: Loader
}

/**
 * @implements DataStructure
 */
const DataStructureConstructor = (library: psi.Library): psi.DataStructure => {
  const DataStructure: psi.DataStructure = library.DataStructure

  /**
   * @interface DataStructure
   */
  return {
    /**
     * Get the 'Raw' enum
     *
     * @function
     * @name DataStructure.Raw
     * @type {DataStructure.Raw}
     */
    get Raw(): psi.DataStructure {
      /**
       * @typedef {DataStructure.Raw} DataStructure.Raw
       */
      return DataStructure.Raw
    },
    /**
     * Get the 'GCS' enum
     *
     * @function
     * @name DataStructure.GCS
     * @type {DataStructure.GCS}
     */
    get GCS(): psi.DataStructure {
      /**
       * @typedef {DataStructure.GCS} DataStructure.GCS
       */
      return DataStructure.GCS
    },
    /**
     * Get the 'BloomFilter' enum
     *
     * @function
     * @name DataStructure.BloomFilter
     * @type {DataStructure.BloomFilter}
     */
    get BloomFilter(): psi.DataStructure {
      /**
       * @typedef {DataStructure.BloomFilter} DataStructure.BloomFilter
       */
      return DataStructure.BloomFilter
    }
  }
}

export const DataStructureWrapperConstructor = ({
  loader
}: DataStructureWrapperOptions): DataStructureWrapper => {
  const library: psi.Library = loader.library
  return DataStructureConstructor(library)
}

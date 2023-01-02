import * as psi from 'psi_'
import { Loader } from '../main/loader'

export type DataStructure = {
  readonly BloomFilter: any
  readonly GCS: any
}

export type DataStructureWrapper = DataStructure

type DataStructureWrapperOptions = {
  readonly loader: Loader
}

/**
 * @implements DataStructure
 */
const DataStructureConstructor = (library: psi.Library): DataStructure => {
  const DataStructure: psi.DataStructure = library.DataStructure

  /**
   * @interface DataStructure
   */
  return {
    /**
     * Get the 'BloomFilter' enum
     *
     * @function
     * @name DataStructure.BloomFilter
     * @type {DataStructure.BloomFilter}
     */
    get BloomFilter(): DataStructure {
      /**
       * @typedef {DataStructure.BloomFilter} DataStructure.BloomFilter
       */
      return DataStructure.BloomFilter
    },
    /**
     * Get the 'GCS' enum
     *
     * @function
     * @name DataStructure.GCS
     * @type {DataStructure.GCS}
     */
    get GCS(): DataStructure {
      /**
       * @typedef {DataStructure.GCS} DataStructure.GCS
       */
      return DataStructure.GCS
    }
  }
}

export const DataStructureWrapperConstructor = ({
  loader
}: DataStructureWrapperOptions): DataStructureWrapper => {
  const library: psi.Library = loader.library
  return DataStructureConstructor(library)
}

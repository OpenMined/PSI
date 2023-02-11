declare module 'psi_*' {
  type Status = {
    readonly Message: string
  }

  type Result = {
    readonly Status?: Status
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    readonly Value: any
  }

  type CreateSetupMessageResult = Result & {
    readonly Value: string
  }
  type CreateRequestResult = Result & {
    readonly Value: string
  }
  type ProcessRequestResult = Result & {
    readonly Value: string
  }
  type GetIntersectionResult = Result & {
    readonly Value: number[]
  }
  type GetIntersectionSizeResult = Result & {
    readonly Value: number
  }
  type CreateClientResult = Result & {
    readonly Value: Client
  }
  type CreateServerResult = Result & {
    readonly Value: Server
  }

  export type Server = {
    readonly delete: () => void
    readonly CreateSetupMessage: (
      fpr: number,
      numClientInputs: number,
      inputs: readonly string[],
      dataStructure: DataStructure
    ) => CreateSetupMessageResult
    readonly ProcessRequest: (clientRequest: Uint8Array) => ProcessRequestResult
    readonly GetPrivateKeyBytes: () => Uint8Array
  }

  export type Client = {
    readonly delete: () => void
    readonly CreateRequest: (
      clientInputs: readonly string[]
    ) => CreateRequestResult
    readonly GetIntersection: (
      serverSetup: Uint8Array,
      serverResponse: Uint8Array
    ) => GetIntersectionResult
    readonly GetIntersectionSize: (
      serverSetup: Uint8Array,
      serverResponse: Uint8Array
    ) => GetIntersectionSizeResult
    readonly GetPrivateKeyBytes: () => Uint8Array
  }

  export type Package = {
    readonly version: () => string
  }

  export type DataStructure = {
    readonly Raw: any
    readonly GCS: any
    readonly BloomFilter: any
  }

  export type Library = {
    readonly delete: () => void
    readonly Package: Package
    readonly DataStructure: DataStructure
    readonly CreateSetupMessage: (
      fpr: number,
      numClientInputs: number,
      inputs: readonly string[],
      dataStructure: DataStructure
    ) => CreateSetupMessageResult
    readonly CreateRequest: (inputs: readonly string[]) => CreateRequestResult
    readonly ProcessRequest: (clientRequest: string) => ProcessRequestResult
    readonly GetIntersection: (
      setup: string,
      response: string
    ) => GetIntersectionResult
    readonly GetIntersectionSize: (
      setup: string,
      response: string
    ) => GetIntersectionSizeResult
    readonly GetPrivateKeyBytes: () => Uint8Array
    readonly PsiClient: {
      readonly CreateWithNewKey: (
        revealIntersection: boolean
      ) => CreateClientResult
      readonly CreateFromKey: (
        key: Uint8Array,
        revealIntersection: boolean
      ) => CreateClientResult
    }
    readonly PsiServer: {
      readonly CreateWithNewKey: (
        revealIntersection: boolean
      ) => CreateServerResult
      readonly CreateFromKey: (
        key: Uint8Array,
        revealIntersection: boolean
      ) => CreateServerResult
    }
  }

  export default function bin(): Promise<Library>
}

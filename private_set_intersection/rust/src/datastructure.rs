#[repr(C)]
#[non_exhaustive]
#[derive(Default, Debug, Copy, Clone, PartialEq, Eq)]
pub enum PsiDataStructure {
    Raw,
    #[default]
    Gcs,
    BloomFilter,
}

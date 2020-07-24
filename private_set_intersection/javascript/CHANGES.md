# Version 0.2.1

## JS

Fix:

- There was a misconfiguration in rollup which did not properly bundle protocol buffers for the brower.

Breaking:

- Deep import links have changed to now support `cjs`, `iife`, and `es`. Notably, `umd` has been removed and superceeded by `cjs` and `iife`, respectively. Users will need to specify the deep import paths for nodejs (`cjs`) and the browser (`iife` or `es`) instead of relying on the previous `umd` build.

# Version 0.2.0

Feat:

- Implemented protocol buffers for data-marshaling for all languages

# Version 0.1.0

Initial release!

Feat:

- Data is passed between client and server as json strings for all languages

## Caveats
Several caveats should be carefully considered before using PSI-Cardinality.

### Information assumed public 
1. Server set size
2. Client set size
(Note that Each of these can be turned into upper bounds by adding dummy elements.)

### Security Limitations for the PSI-Cardinality protocol

Coordinated clients could get the actual intersection. However, server set items not
in any of the client sets will never be uncovered.
Situations where it’s feasible for clients to send one request per element in the domain -
there is a possbility that coordinated clients could uncover server set. A larger set means
more bits turned on in bloom filter.


Presence of new client set members or absence of former client set members can be
detected by server/eavesdroppers if client secret is reused.


In the absence of any rate limiting and assuming the client and server have enough
computing power and bandwidth, small domains may be brute-forceable. However, a query
needs to be  performed for each brute-force attempt.
An example for this situation would be suppose you were trying to limit sending antibody 
tests to people based on whether they’d been in an infected location, so that people would
have to share their location history to prove they’d been somewhere infected, and you were
using PSI so people wouldn’t have to share their location history without good reason. If
your health authority only covers 10 possible geohashes, people could sidestep the PSI step
entirely and submit location histories which unlock tests by brute force.


A potential limitation with the PSI-cardinality approach is the communication complexity,
which scales linearly with the size of the larger set. This is of particular concern
when performing PSI between a constrained device (cellphone) holding a small set, and a
large service provider (e.g. WhatsApp), such as in the Private Contact Discovery application.
The Client set size affects the algorithmic complexity in linear time O(n).
The bloom filter has linear size in the server's set, hence the algorithmic complexity
of our protocol is O(n).

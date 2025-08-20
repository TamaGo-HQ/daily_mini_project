thread test resul :
"Running threadsafe test..."
./tests/test_threadsafe
=== Thread-Safe Circular Buffer Tests ===
Testing basic functionality with thread-safe buffer...
Γ£ô Basic functionality works with thread-safe buffer
Testing multi-threaded access...
Starting 2 producers and 2 consumers...
Consumer 1: 99/100 operations successful
Producer 0: 100/100 operations successful
Consumer 0: 99/100 operations successful
Producer 1: 100/100 operations successful
Total produced: 200, Total consumed: 198, Remaining in buffer: 2
Γ£ô Multi-threaded test passed - no data corruption detected!

≡ƒÄë All thread-safe tests passed!
Your circular buffer is ready for embedded systems!
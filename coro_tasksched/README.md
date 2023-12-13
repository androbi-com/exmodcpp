# coro_tasksched

This example builds on the `coro_task` example and implements a scheduler
as shown by Dian-Lun Lin in https://www.youtube.com/watch?v=kIPzED3VD3w&ab_channel=CppNow

A scheduler is added that creates a queue (or stack) of coroutine handles. These
handles are used to resume the coroutines until they are done.
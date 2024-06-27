### Chrono

包含`pybind11/chrono.h`将使能C++11 chrono和Python datatime对象将的自动转换，
还支持python floats（从time.monotonic()或time.perf_counter()获取的）和time.process_time()到durations的转换。

**C++11时钟的概览**

使用这些转换时容易混淆的点是，C++11中提供的各种时钟的差异。
C++11标准定义了三种时钟类型，用户也可以根据自身需求定义自己的时钟类型。
这些时钟有着不用的属性，与Python之间转换时也会获得不同的结果。

标准定义的第一种时钟std::chrono::system_clock。它测量当前的时间和日期。
但是，这个时钟会随着操作系统的时钟变化而改变。例如，在系统时间与时间服务器同步时，这个时钟也会跟着改变。
这对计时功能来说很糟糕，但对测量wall time还是有用的。

标准定义的第二种时钟std::chrono::steady_clock。这个时钟以稳定的速度跳动，从不调整。
这非常实用于计时功能，但与实际时间和日志并不一致。这个时间通常是你操作系统已经运行的时间，虽然不是必须的。
这个时钟永远不会与系统时钟相同，因为系统时钟可以改变，但steady_clock不能。

标准定义的第三种时钟std::chrono::high_resolution_clock。
它是系统中分辨率最高的时钟，通常是system clock 或 steady clock的一种，也可以有自己独立的时钟。
需要注意的是，你在Python中获取到的该时钟的转换值，可能存在差异，这取决于系统的实现。
如果它是系统时钟的一种，Python将得到datetime对象，否则将得到timedelta对象。

**提供的转换**

- C++到Python
    + std::chrono::system_clock::time_point → datetime.datetime
    + std::chrono::duration → datetime.timedelta
    + std::chrono::[other_clocks]::time_point → datetime.timedelta

- Python到C++
    + datetime.datetimeordatetime.dateordatetime.time → std::chrono::system_clock::time_point
    + datetime.timedelta → std::chrono::duration
    + datetime.timedelta → std::chrono::[other_clocks]::time_point
    + float → std::chrono::duration
    + float → std::chrono::[other_clocks]::time_point

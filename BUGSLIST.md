# bugs list for kernel 

## <2020.7.2>
```
1. sendto can work only 1 time.

```
## <2020.8.7>
```
1. 经常出现错误：
assert(!list_find(&task->list, &wait_queue->wait_list)) failed:
file: task/waitqueue.c
base_file: task/waitqueue.c
ln: 17
spinning in assertion failure()

```
## <2021.2.1>
1. 视图刷新时，如果配置了ALPHA计算，在调整视图大小时，如果同时调整宽度和高度，会出现调整后直接变成透明的问题。

## <2021.2.19>
1. lite文本显示问题,怀疑是文本矩阵转换时导致。
2. lite鼠标框选时跳动，鼠标点击位置不太对。

## <2021.2.6>
1. sh执行cd命令时出现bug，群小满同学提出的BUG。
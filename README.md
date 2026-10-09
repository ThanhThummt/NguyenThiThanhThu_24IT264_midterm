# Midterm Project: Implementation of UNIX ls(1)

- **Họ và tên:** Nguyễn Thị Thanh Thu
- **Mã số sinh viên:** 24IT264
- **Môn học:** Lập trình Hệ thống (System Programming)
- **Môi trường thực thi:** NetBSD 10.1 (Oracle VirtualBox)

---

## 1. Tổng quan dự án

Dự án cài đặt phiên bản đơn giản hóa của lệnh UNIX `ls(1)` bằng ngôn ngữ C theo chuẩn mô-đun (Modular C) và tuân thủ các quy định trong tài liệu hướng dẫn (`ls.pdf` - NetBSD General Commands Manual).

## 2. Cấu trúc Mã nguồn (Modular Structure)

- `main.c`: Điểm khởi đầu chương trình, tiếp nhận tham số dòng lệnh.
- `options.h` / `options.c`: Phân tích cờ tùy chọn (`getopt`) và xử lý quy tắc ghi đè (overrides).
- `entry.h` / `entry.c`: Quản lý bộ nhớ danh sách file, đọc dữ liệu hệ thống bằng `lstat()`, `opendir()`, `readdir()`.
- `sort.h` / `sort.c`: Sắp xếp danh sách file theo tên, thời gian (`-t`), kích thước (`-S`), hoặc đảo ngược (`-r`).
- `display.h` / `display.c`: Định dạng và in danh sách (`-l`, `-n`, `-F`, `-h`, `-k`, `-i`, `-s`, `-q`, `-w`).
- `Makefile`: Biên dịch tự động dự án với cờ `-Wall -Wextra -std=c99 -g`.
- `.gitignore`: Bỏ qua các file thực thi và object files (`.o`).

## 3. Các tùy chọn đã cài đặt (Implemented Options)

| Tùy chọn   | Mô tả tính năng                                                         |
| :--------- | :---------------------------------------------------------------------- |
| `-a`, `-A` | Hiển thị tất cả file ẩn / Bỏ qua `.` và `..`                            |
| `-l`, `-n` | Định dạng danh sách chi tiết (Long format) dạng Tên / Dạng số UID & GID |
| `-F`       | Thêm ký hiệu phân loại loại file (`*`, `/`, `@`, `=`, `\|`)             |
| `-h`, `-k` | Định dạng dung lượng dễ đọc (Human readable) / Kilobytes                |
| `-i`, `-s` | In số Inode / In số file system blocks                                  |
| `-t`, `-S` | Sắp xếp theo thời gian sửa đổi / Dung lượng file                        |
| `-r`, `-f` | Đảo ngược thứ tự sắp xếp / Tắt sắp xếp                                  |
| `-R`, `-d` | Duyệt đệ quy thư mục con / Xem thư mục như file thường                  |
| `-q`, `-w` | Thay ký tự không in được bằng `?` / In raw                              |
| `-C`, `-u` | Dùng status change time (ctime) / access time (atime)                   |

## 4. Hướng dẫn Biên dịch và Sử dụng

### Biên dịch dự án:

```bash
make
```

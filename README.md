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
# Biên dịch tạo ra file thực thi ./ls
make

# Dọn dẹp các file object (.o) và file thực thi
make clean
```

### Cú pháp chạy chương trình:

```bash
./ls [OPTIONS] [FILE/DIRECTORY...]
```

---

## 5. Hướng dẫn sử dụng chi tiết & Kết quả mẫu (Examples & Expected Output)

### 5.1. Lệnh cơ bản không truyền cờ (Liệt kê thư mục hiện tại)
- **Cách gõ:**
  ```bash
  ./ls
  ```
- **Kết quả đầu ra mẫu:**
  ```text
  Makefile
  README.md
  display.c
  display.h
  entry.c
  entry.h
  main.c
  options.c
  options.h
  sort.c
  sort.h
  ```

---

### 5.2. Hiển thị file ẩn (`-a` và `-A`)
- **Tùy chọn `-a` (Hiện tất cả bao gồm `.` và `..`):**
  - **Cách gõ:** `./ls -a`
  - **Kết quả đầu ra mẫu:**
    ```text
    .
    ..
    .gitignore
    Makefile
    README.md
    main.c
    ```
- **Tùy chọn `-A` (Hiện file ẩn nhưng bỏ qua `.` và `..`):**
  - **Cách gõ:** `./ls -A`
  - **Kết quả đầu ra mẫu:**
    ```text
    .gitignore
    Makefile
    README.md
    main.c
    ```

---

### 5.3. Định dạng chi tiết (Long format: `-l` và `-n`)
- **Tùy chọn `-l` (Hiển thị chi tiết quyền, owner/group dưới dạng tên, dung lượng, thời gian, tên file):**
  - **Cách gõ:** `./ls -l`
  - **Kết quả đầu ra mẫu:**
    ```text
    total 32
    -rw-r--r--  1 student  student       284 Oct  9 13:47 Makefile
    -rw-r--r--  1 student  student      2743 Oct  9 13:47 README.md
    -rw-r--r--  1 student  student      4281 Oct  9 13:47 display.c
    -rw-r--r--  1 student  student       569 Oct  9 13:47 main.c
    ```
- **Tùy chọn `-n` (Tương tự `-l` nhưng hiển thị UID và GID dạng số):**
  - **Cách gõ:** `./ls -n`
  - **Kết quả đầu ra mẫu:**
    ```text
    total 32
    -rw-r--r--  1 1000     1000          284 Oct  9 13:47 Makefile
    -rw-r--r--  1 1000     1000         2743 Oct  9 13:47 README.md
    -rw-r--r--  1 1000     1000         4281 Oct  9 13:47 display.c
    -rw-r--r--  1 1000     1000          569 Oct  9 13:47 main.c
    ```

---

### 5.4. Ký hiệu phân loại loại file (`-F`)
- **Cách gõ:** `./ls -F`
- **Mô tả:** Thêm dấu `/` cho thư mục, `*` cho file thực thi, `@` cho symlink, `=` cho socket, `|` cho FIFO.
- **Kết quả đầu ra mẫu:**
  ```text
  Makefile
  README.md
  build/
  ls*
  main.c
  ```

---

### 5.5. Định dạng dung lượng (`-h` và `-k`)
- **Tùy chọn `-h` (Human-readable: B, K, M, G):**
  - **Cách gõ:** `./ls -lh`
  - **Kết quả đầu ra mẫu:**
    ```text
    total 32
    -rw-r--r--  1 student  student     284B Oct  9 13:47 Makefile
    -rw-r--r--  1 student  student     2.7K Oct  9 13:47 README.md
    -rw-r--r--  1 student  student     4.2K Oct  9 13:47 display.c
    ```
- **Tùy chọn `-k` (Hiển thị block sizes / dung lượng theo Kilobytes):**
  - **Cách gõ:** `./ls -sk`
  - **Kết quả đầu ra mẫu:**
    ```text
    total 16
       4 Makefile
       4 README.md
       8 display.c
    ```

---

### 5.6. In số Inode và Block (`-i` và `-s`)
- **Cách gõ:** `./ls -li`
- **Kết quả đầu ra mẫu:**
  ```text
  total 32
  1452981 -rw-r--r--  1 student  student       284 Oct  9 13:47 Makefile
  1452982 -rw-r--r--  1 student  student      2743 Oct  9 13:47 README.md
  1452983 -rw-r--r--  1 student  student      4281 Oct  9 13:47 display.c
  ```

---

### 5.7. Sắp xếp (`-t`, `-S`, `-r`, `-f`)
- **Sắp xếp theo thời gian sửa đổi gần nhất (`-t`):**
  - **Cách gõ:** `./ls -lt`
- **Sắp xếp theo kích thước giảm dần (`-S`):**
  - **Cách gõ:** `./ls -lS`
  - **Kết quả đầu ra mẫu:**
    ```text
    total 32
    -rw-r--r--  1 student  student      4281 Oct  9 13:47 display.c
    -rw-r--r--  1 student  student      3309 Oct  9 13:47 entry.c
    -rw-r--r--  1 student  student      2743 Oct  9 13:47 README.md
    -rw-r--r--  1 student  student       284 Oct  9 13:47 Makefile
    ```
- **Đảo ngược thứ tự sắp xếp (`-r`):**
  - **Cách gõ:** `./ls -lr` (sắp xếp tên ngược từ z -> a) hoặc `./ls -ltr` (thời gian cũ nhất lên đầu)
- **Tắt sắp xếp, in theo thứ tự đọc trong thư mục (`-f`):**
  - **Cách gõ:** `./ls -f`

---

### 5.8. Đệ quy và Thư mục (`-R` và `-d`)
- **Tùy chọn `-R` (Duyệt đệ quy tất cả cây thư mục con):**
  - **Cách gõ:** `./ls -R`
  - **Kết quả đầu ra mẫu:**
    ```text
    Makefile
    README.md
    src

    ./src:
    display.c
    display.h
    main.c
    ```
- **Tùy chọn `-d` (Xem thư mục như file thông thường, không mở thư mục ra):**
  - **Cách gõ:** `./ls -ld src`
  - **Kết quả đầu ra mẫu:**
    ```text
    drwxr-xr-x  2 student  student      4096 Oct  9 13:47 src
    ```

---

### 5.9. Quản lý thời gian (`-C` và `-u`) kết hợp với `-l` hoặc `-t`
- **Tùy chọn `-C`:** Sử dụng thời gian thay đổi trạng thái file (inode status change time - `ctime`).
  - **Cách gõ:** `./ls -lc` hoặc `./ls -ltC`
- **Tùy chọn `-u`:** Sử dụng thời gian truy cập file lần cuối (access time - `atime`).
  - **Cách gõ:** `./ls -lu` hoặc `./ls -ltu`

---

### 5.10. Kết hợp nhiều tùy chọn (Complex Flag Combinations)
- **Ví dụ kết hợp xem toàn diện danh sách:**
  - **Cách gõ:** `./ls -laFh`
  - **Ý nghĩa:** Liệt kê đầy đủ file ẩn (`-a`), dạng dài (`-l`), dung lượng dễ đọc (`-h`), ký hiệu phân loại loại file (`-F`).
  - **Kết quả đầu ra mẫu:**
    ```text
    total 40
    drwxr-xr-x  2 student  student     4.0K Oct  9 13:47 ./
    drwxr-xr-x  4 student  student     4.0K Oct  9 13:00 ../
    -rw-r--r--  1 student  student      17B Oct  9 13:48 .gitignore
    -rwxr-xr-x  1 student  student    18.2K Oct  9 13:49 ls*
    -rw-r--r--  1 student  student     284B Oct  9 13:47 Makefile
    -rw-r--r--  1 student  student     4.5K Oct  9 13:50 README.md
    ```

//
// Created by o0kam1 on 2025/05/12.
//

#ifndef UNISTD_H
#define UNISTD_H

#ifdef __cplusplus
extern "C" {
#endif

int unlink(const char* path);
int isatty(int fd);

#ifdef __cplusplus
}
#endif

#endif //UNISTD_H

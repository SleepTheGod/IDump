```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

static void print_file(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        printf("[ ] %s → not found or inaccessible (%s)\n", path, strerror(errno));
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        printf("[ ] %s → is a directory, skipping\n", path);
        return;
    }

    FILE *f = fopen(path, "r");
    if (!f) {
        printf("[ ] %s → cannot open (%s)\n", path, strerror(errno));
        return;
    }

    printf("\n[+] %s (size: %ld bytes, uid:%d gid:%d mode:0%o)\n", 
           path, (long)st.st_size, st.st_uid, st.st_gid, st.st_mode & 07777);
    printf("--------------------------------------------------\n");

    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf)-1, f)) > 0) {
        buf[n] = '\0';
        printf("%s", buf);
    }
    fclose(f);
    printf("\n--------------------------------------------------\n\n");
}

int main(void) {
    printf("Internal server config dumper (run as root for best results)\n");
    printf("Current euid: %d   egid: %d\n\n", geteuid(), getegid());

    const char *configs[] = {
        "/etc/passwd",
        "/etc/shadow",
        "/etc/group",
        "/etc/sudoers",
        "/etc/sudoers.d/README",
        "/etc/apache2/apache2.conf",
        "/etc/apache2/sites-enabled/000-default.conf",
        "/etc/nginx/nginx.conf",
        "/etc/nginx/sites-enabled/default",
        "/etc/mysql/my.cnf",
        "/etc/mysql/mariadb.conf.d/50-server.cnf",
        "/etc/postgresql/*/main/postgresql.conf",
        "/etc/ssh/sshd_config",
        "/etc/redis/redis.conf",
        "/etc/docker/daemon.json",
        "/var/www/html/config.php",
        "/etc/hosts",
        "/proc/cpuinfo",
        "/proc/meminfo",
        NULL
    };

    for (int i = 0; configs[i]; i++) {
        print_file(configs[i]);
    }

    return 0;
}```

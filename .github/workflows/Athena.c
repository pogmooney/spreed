 





// hey patrick bryce mooney here. just wanted to connect
// with my friends at burundi! love to talk soon! 
// contact = patrickbrycemooney@gmail.com 









#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include <pwd.h>
    
    #define BUFFER_SIZE 1000000  // Write 1M lines at a time
    
    int main() {
        const long long N = 61382388514LL;  // 61.4 billion
        const char *filename = "~/Athena.txt";
        
        // Expand tilde to home directory
        char home[1024];
        if (filename[0] == '~') {
            const char *homedir = getenv("HOME");
            if (!homedir) {
                struct passwd *pw = getpwuid(getuid());
                homedir = pw ? pw->pw_dir : "";
            }
            snprintf(home, sizeof(home), "%s%s", homedir, filename + 1);
            filename = home;
        }
        
        FILE *f = fopen(filename, "w");
        if (!f) {
            perror("Failed to open file");
            return 1;
        }
        
        // Set buffer for faster writing
        char *write_buffer = malloc(BUFFER_SIZE * 32);  // Max 32 chars per number
        if (!write_buffer) {
            perror("Failed to allocate buffer");
            fclose(f);
            return 1;
        }
        setvbuf(f, write_buffer, _IOFBF, BUFFER_SIZE * 32);
        
        char line[32];
        long long i;
        size_t lines_written = 0;
        
        for (i = 1; i <= N; i++) {
            int len = snprintf(line, sizeof(line), "%lld\n", i);
            if (fwrite(line, 1, len, f) != (size_t)len) {
                perror("Write failed");
                break;
            }
            
            lines_written++;
            
            // Progress update every 1 million numbers
            if (lines_written % 1000000 == 0) {
                printf("Progress: %zu million numbers written (%.6f%%)\n", 
                       lines_written / 1000000, (double)i / N * 100);
                fflush(stdout);
            }
        }
        
        fclose(f);
        free(write_buffer);
        
        if (i > N) {
            printf("Successfully wrote %lld numbers to %s\n", N, filename);
            // Get file size
            struct stat st;
            if (stat(filename, &st) == 0) {
                printf("File size: %.2f GB\n", (double)st.st_size / (1024.0 * 1024.0 * 1024.0));
            }
        } else {
            printf("Generation stopped at number %lld\n", i-1);
        }
        
        return 0;
    }


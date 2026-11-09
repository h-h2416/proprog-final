#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h> 
#include <errno.h>


#define ALPHABET_SIZE 26

typedef struct HashNode {
    char *word;
    int count;
    int length;
    struct HashNode *next;
    int longest;
    int shortest;
    int position[5]; 
} HashNode;

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int is_end_of_word;
    int position[5];
} TrieNode;

typedef struct WordList {
    char **word;
    int count;
    int **position;
    int capacity;
} WordList;

void show_usage(void);
unsigned long hash_index(const char *str);
int hash_table_check(HashNode *hash_table[], const char *buffer);
int hash_table_add(HashNode *hash_table[], const char *buffer , int length, int total_characters);
void trie_search_word(WordList *list, TrieNode *node, int depth, char *buffer);
TrieNode* create_TrieNode(void);
void swap(HashNode **a, HashNode **b);
void trie_insert(HashNode *hash_node[], TrieNode *root, char *word, int length);
TrieNode *trie_search_prefix(TrieNode *root, const char *prefix);
void heap_add(HashNode *heap[], HashNode *node);
void heap_shiftdown(HashNode *heap[], int heap_size, int target);
HashNode *heap_remove(HashNode *heap[], int n);

int main(int argc, char *argv[]) {
    int i = 2;
    char *endptr;
    int s_tf = 0;
    int t_tf = 0;
    int f_tf = 0;
    int w_tf = 0;
    int total_characters = 0;
    int prefix_index = 0;
    int word_index = 0;
    int word_length = 0;
    long n = 0;
    char *prefix[100] = {NULL};
    char *word[100] = {NULL};
    FILE *fp = fopen(argv[1], "r");

    if (!fp) {
        perror("Failed to open file");
        show_usage();
        fclose(fp);
        return 1;
    }

    while (i < argc) {
        if (strcmp(argv[i], "-s") == 0) {
            i++;
            s_tf = 1;
        }else if (strcmp(argv[i], "-t") == 0) {
            if (i + 1 >= argc) {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
            n = strtol(argv[i + 1], &endptr, 10);
            t_tf = 1;
            if (errno == ERANGE || n < 1 || *endptr != '\0') {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
            i += 2;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc) {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
            if (argv[i + 1][0] == '\0') {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
            if (strtol(argv[i + 1], &endptr, 10) == 0 && *endptr == argv[i + 1][0]) {
                if (prefix_index >= 100) {
                    printf("Too many prefixes specified\n");
                    show_usage();
                    fclose(fp);
                    for (int j = 0; j < prefix_index; j++) {
                        free(prefix[j]);
                    }
                    for (int j = 0; j < word_index; j++) {
                        free(word[j]);
                    }
                    return 1;
                }
                prefix[prefix_index] = (char *)malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                prefix[prefix_index] = argv[i + 1];
                prefix_index++;
                i += 2;
                f_tf = 1;
            } else {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
        } else if (strcmp(argv[i], "-w") == 0) {
            if (i + 1 >= argc) {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
            if (argv[i + 1][0] == '\0') {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
            if (strtol(argv[i + 1], &endptr, 10) == 0 && *endptr == argv[i + 1][0]) {
                if (word_index >= 100) {
                    printf("Too many words specified\n");
                    show_usage();
                    fclose(fp);
                    for (int j = 0; j < prefix_index; j++) {
                        free(prefix[j]);
                    }
                    for (int j = 0; j < word_index; j++) {
                        free(word[j]);
                    }
                    return 1;
                }
                word[word_index] = (char *)malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
                word[word_index] = argv[i + 1];
                word_index++;
                w_tf = 1;
                i += 2;
            } else {
                printf("Invalid option\n");
                show_usage();
                fclose(fp);
                for (int j = 0; j < prefix_index; j++) {
                    free(prefix[j]);
                }
                for (int j = 0; j < word_index; j++) {
                    free(word[j]);
                }
                return 1;
            }
        }else {
            printf("Invalid option\n");
            show_usage();
            fclose(fp);
            for (int j = 0; j < prefix_index; j++) {
                free(prefix[j]);
            }
            for (int j = 0; j < word_index; j++) {
                free(word[j]);
            }
            return 1;
        }
    }
    if (s_tf == 0 && t_tf == 0 && f_tf == 0 && w_tf == 0) {
        printf("No options specified\n");
        show_usage();
        fclose(fp);
        for (int j = 0; j < prefix_index; j++) {
            free(prefix[j]);
        }
        for (int j = 0; j < word_index; j++) {
            free(word[j]);
        }
        return 1;
    }
    printf("s_tf: %d\nt_tf: %d\nn = %ld\nf_tf: %d\nprefix ="
        ,s_tf, t_tf, n, f_tf);
    if (prefix_index != 0) {
        for (int j = 0; j < prefix_index - 1; j++) {
            printf(" %s,",prefix[j]);
        }
        printf(" %s\n", prefix[prefix_index - 1]);
    } else {
        printf("\n");
    }
    
    printf("w_tf: %d\nword =",w_tf);
    if (word_index != 0) {
        for (int j = 0; j < word_index - 1; j++) {
            printf(" %s,",word[j]);
        }
        printf(" %s\n", word[word_index - 1]);
    }
    

    // ここまでが入力に対する例外処理．あとはオプションの処理

    HashNode *hash_table[1000] = {NULL};
    if (s_tf || t_tf || f_tf || w_tf) {
        // ハッシュテーブルを構築
        char buffer[256] = {0};
        int index = 0;
        int chara = 0;
        while (1) {
            if (chara == EOF) {
                break;
            }
            while (isalpha(chara = fgetc(fp))) {
                total_characters++;
                buffer[index] = tolower(chara);
                index++;
                if (index >= 255) {
                    fprintf(stderr, "Buffer overflow detected\n");
                    fclose(fp);
                    for (int j = 0; j < prefix_index; j++) {
                        free(prefix[j]);
                    }
                    for (int j = 0; j < word_index; j++) {
                        free(word[j]);
                    }
                    for (int j = 0; j < 1000; j++) {
                        HashNode *node = hash_table[j];
                        while (node != NULL) {
                            HashNode *temp = node;
                            node = node->next;
                            free(temp->word);
                            free(temp);
                        }
                    }
                    return 1;
                }
            }
            total_characters++;
            if (index > 0) {
                if (hash_table_check(hash_table, buffer) == 0) {
                    // printf("DEBUG: total_characters=%d, word_length=%d, position=%d\n", total_characters, word_length, total_characters - word_length);
                    word_length = strlen(buffer) + 1; 
                    int check = hash_table_add(hash_table, buffer, index, total_characters - word_length);
                    if (check == -1) {
                        fprintf(stderr, "Failed to add word to hash table\n");
                        fclose(fp);
                        for (int j = 0; j < prefix_index; j++) {
                            free(prefix[j]);
                        }
                        for (int j = 0; j < word_index; j++) {
                            free(word[j]);
                        }
                        for (int j = 0; j < 1000; j++) {
                            HashNode *node = hash_table[j];
                            while (node != NULL) {
                                HashNode *temp = node;
                                node = node->next;
                                free(temp->word);
                                free(temp);
                            }
                        }
                        return 1;
                    }
                    index = 0;
                    memset(buffer, 0, sizeof(buffer));
                } else {
                    int target_index = hash_index(buffer);
                    index = 0;
                    HashNode *node = hash_table[target_index];
                    while (strcmp(node->word, buffer) != 0) {
                        node = node->next;
                    }
                    node->count++;
                    if (node->count < 6) {
                        node->position[node->count - 1] = total_characters - word_length;
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
            } else {
                index = 0;
                continue;
            }
            
        }
    }
    // ここまででハッシュテーブル完成

/*
    for (int show_index = 0; show_index < 1000; show_index++) {
        HashNode *node = hash_table[show_index];
        while (node != NULL) {
            printf("Word: %s, Count: %d, Length: %d\n", node->word, node->count, node->length);
            node = node->next;
        }
    }
*/

    if (s_tf) {
        int total_words = 0;
        int unique_words = 0;
        int total_words_length = 0;
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            while (node != NULL) {
                total_words_length += node->length * node->count;
                total_words += node->count;
                unique_words++;
                node = node->next;
            }
        }
        printf("=== TEXT STATISTICS ===\n");
        if (total_characters - 1 == 1) {
            printf("Total character: %d\n", total_characters - 1);
        } else {
            printf("Total characters: %d\n", total_characters - 1);
        }

        if (total_words == 1) {
            printf("Total word: %d\n", total_words);
        } else {
            printf("Total words: %d\n", total_words);
        }

        if (unique_words == 1) {
            printf("Unique word: %d\n", unique_words);
        } else {
            printf("Unique words: %d\n", unique_words);
        }

        float average_word_length = (float)total_words_length / total_words;
        printf("Average word length: %.2f\n", average_word_length);
        int longest_word_length = 0;
        int longest_word_count = 0;
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            while (node != NULL) {
                if (node->length == longest_word_length) {
                    longest_word_count++;
                }
                if (node->length > longest_word_length) {
                    longest_word_length = node->length;
                    longest_word_count = 1;
                }
                node = node->next;
            }
        }
        if (longest_word_count == 1) {
            printf("Longest word:\n");
        } else {
            printf("Longest words:\n");
        }
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            while (node != NULL) {
                if (node->length == longest_word_length) {
                    printf("\"%s\"\n", node->word);
                }
                node = node->next;
            }
        }

        if (longest_word_length == 1) {
            printf("(%d character)\n", longest_word_length);
        } else {
            printf("(%d characters)\n", longest_word_length);
        }

        int shortest_word_length = 256;
        int shortest_word_count = 0;
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            while (node != NULL) {
                if (node->length == shortest_word_length) {
                        shortest_word_count++;
                }
                if (node->length < shortest_word_length) {
                    shortest_word_length = node->length;
                    shortest_word_count = 1;
                }
                node = node->next;
            }
        }

        if (shortest_word_count == 1) {
            printf("Shortest word:\n");
        } else {
            printf("Shortest words:\n");
        }
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            while (node != NULL) {
                if (node->length == shortest_word_length) {
                    printf("\"%s\"\n", node->word);
                }
                node = node->next;
            }
        }
        if (shortest_word_length == 1) {
            printf("(%d character)\n", shortest_word_length);
        } else {
            printf("(%d characters)\n", shortest_word_length);
        }
    }
    
    if (f_tf) {
        TrieNode *root = create_TrieNode();
        if (root == NULL) {
            fprintf(stderr, "Failed to create TrieNode\n");
            fclose(fp);
            for (int j = 0; j < prefix_index; j++) {
                free(prefix[j]);
            }
            for (int j = 0; j < word_index; j++) {
                free(word[j]);
            }
            for (int j = 0; j < 1000; j++) {
                HashNode *node = hash_table[j];
                while (node != NULL) {
                    HashNode *temp = node;
                    node = node->next;
                    free(temp->word);
                    free(temp);
                }
            }
            return 1;
        }
        // トライ木を構築
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            while (node != NULL) {
                trie_insert(hash_table, root, node->word, node->length);
                node = node->next;
            }
        }
        // 接頭辞の深さ優先探索と出力
        WordList *list = (WordList *)malloc(sizeof(WordList));
        if (list == NULL) {
            perror("Failed to malloc for WordList");
            fclose(fp);
            for (int k = 0; k < prefix_index; k++) {
                free(prefix[k]);
            }
            for (int k = 0; k < word_index; k++) {
                free(word[k]);
            }
            for (int k = 0; k < 1000; k++) {
                HashNode *node = hash_table[k];
                while (node != NULL) {
                    HashNode *temp = node;
                    node = node->next;
                    free(temp->word);
                    free(temp);
                }
            }
            free(list);
            free(root);
            return 1;
        }
        for (int j = 0; j < prefix_index; j++) {
            // 深さ優先探索
            TrieNode *prefix_node = trie_search_prefix(root, prefix[j]);
            if (prefix_node == NULL) {
                printf("No words found starting with \"%s\"\n", prefix[j]);
                continue;
            }
            
            char buffer[256] = {0};
            int k = 0;
            for (k = 0; prefix[j][k] != '\0'; k++) {
                buffer[k] = prefix[j][k];
            }
            list->count = 0;
            int t = 10;
            while (k > t) {
                t += 10;
            }
            list->capacity = t;
            list->word = (char **)malloc(sizeof(char *) * list->capacity);
            list->position = (int **)malloc(sizeof(int *) * list->capacity);
            if (list->word == NULL || list->position == NULL) {
                perror("Failed to malloc for word array");
                free(list);
                fclose(fp);
                for (int index = 0; index < prefix_index; index++) {
                    free(prefix[index]);
                }
                for (int index = 0; index < word_index; index++) {
                    free(word[index]);
                }
                for (int index = 0; index < 1000; index++) {
                    HashNode *node = hash_table[index];
                    while (node != NULL) {
                        HashNode *temp = node;
                        node = node->next;
                        free(temp->word);
                        free(temp);
                    }
                }
                free(root);
                return 1;
            }
            trie_search_word(list, prefix_node, k, buffer);
            printf("=== WORDS STARTING WITH \"%s\" ===\n", prefix[j]);
            printf("Found %d matches:\n", list->count);
            for (int index = 0; index < list->count; index++) {
                printf("-\"%s\" (position ", list->word[index]);
                int l = 0;
                for (l = 0; l < 3 && list->position[index][l + 1] != -1; l++) {
                    printf("%d, ", list->position[index][l]);
                }
                if (list->position[index][4] != -1) {
                    printf("%d ...)\n", list->position[index][3]);
                } else {
                    printf("%d)\n", list->position[index][l]);
                }
            }
        }
        free(root);
        for (int j = 0; j < prefix_index; j++) {
            free(prefix[j]);
        }
        for (int j = 0; j < list->count; j++) {
            free(list->word[j]);
            free(list->position[j]);
        }
        free(list->word);
        free(list->position);
        free(list);
    }
        
    if (t_tf) {
        /*
        親ノードのインデックス: (i - 1) / 2
        左の子ノードのインデックス: 2 * i + 1
        右の子ノードのインデックス: 2 * i + 2
        */
        int root_exists = 0;
        HashNode **heap;
        heap = (HashNode **)malloc(sizeof(HashNode *) * n);
        if (heap == NULL) {
            perror("Failed to malloc for heap");
            fclose(fp);
            for (int j = 0; j < prefix_index; j++) {
                free(prefix[j]);
            }
            for (int j = 0; j < word_index; j++) {
                free(word[j]);
            }
            for (int j = 0; j < 1000; j++) {
                HashNode *node = hash_table[j];
                while (node != NULL) {
                    HashNode *temp = node;
                    node = node->next;
                    free(temp->word);
                    free(temp);
                }
            }
            free(heap);
            
            return 1;
        }
        for (int index = 0; index < n; index++) {
            heap[index] = NULL;
        }
        
        for (int j = 0; j < 1000; j++) {
            HashNode *node = hash_table[j];
            if (node == NULL) {
                continue;
            } else {
                while (node != NULL) {
                    if (root_exists == 0) {
                        heap[0] = node;
                        root_exists++;
                        node = node->next;
                    } else if (heap[n - 1] == NULL) {
                        heap_add(heap, node);
                        node = node->next;
                    } else if (node->count > heap[0]->count) {
                        heap[0] = node;
                        heap_shiftdown(heap, n, 0);
                        node = node->next;
                    } else {
                        node = node->next;
                    }
                }
            }
        }
        for (int index = 0; index < n - 1; index++) {
            for (int k = 0; k < n - index - 1; k++) {
                if (heap[k + 1] == NULL) {
                    continue;
                }else if (heap[k]->count < heap[k + 1]->count) {
                    swap(&heap[k], &heap[k + 1]);
                }
                
            }
        }
        printf("=== TOP %ld WORDS ===\n", n);
        int index = 0;
        while (index < n && heap[index] != NULL) {
            const char *occurrence_string = (heap[index]->count == 1) ? "occurrence" : "occurrences";
            printf("%d. \"%s\" - %d %s\n", index + 1, heap[index]->word, heap[index]->count, occurrence_string);
            index++;
        }
        for (int j = 0; j < n; j++) {
            if (heap[j] != NULL) {
                free(heap[j]->word);
                free(heap[j]);
            }
        }
        free(heap);
    }

    if (w_tf) {
        for (int j = 0; j < word_index; j++) {
            int found = 0;
            printf("=== WORD SEARCH ===\n");
            for (int k = 0; k < 1000; k++) {
                HashNode *node = hash_table[k];
                while (node != NULL) {
                    if (strcmp(node->word, word[j]) == 0) {
                        found = 1;
                        const char *occurrence_string = node->count == 1 ? "occurrence" : "occurrences";
                        const char *position_string = node->count == 1 ? "position" : "positions";
                        printf("Word \"%s\" found: %d %s (%s ", node->word, node->count, occurrence_string, position_string);
                        for (int l = 0; l < node->count - 1 && l < 3; l++) {
                            printf("%d, ", node->position[l]);
                        }
                        if (node->count > 4) {
                            printf("%d ...)\n", node->position[4]);
                        } else {
                            printf("%d)\n", node->position[node->count - 1]);
                        }
                    }
                    node = node->next;
                }
            }
            if (!found) {
                printf("Word \"%s\" not found\n", word[j]);
            }
        }
    }
    fclose(fp);
    for (int j = 0; j < word_index; j++) {
        free(word[j]);
    }
    for (int j = 0; j < 1000; j++) {
        HashNode *node = hash_table[j];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp->word);
            free(temp);
        }
    } 
}


void show_usage(void) {
    printf("./textproc <input_file> [options]\n");
    printf("Options:\n");
    printf("  -s             Show statistics\n");
    printf("  -f <prefix>    Find words starting with prefix\n");
    printf("  -t <n>         Show top n most frequent words\n");
    printf("  -w <word>      Search for specific word\n");
    return;
}

unsigned long hash_index(const char *str) {
    unsigned long hash = 5381;
    int c;
    int table_size = 1000; 
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    int hash_index = hash % table_size;
    return hash_index;
}

// ハッシュテーブルに同じ文字列が存在するか確認する関数
int hash_table_check(HashNode *hash_table[], const char *buffer) {
    unsigned long index = hash_index(buffer);
    HashNode *node = hash_table[index];
    
    while (node != NULL) {
        if (strcmp(node->word, buffer) == 0) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

// ハッシュテーブルに文字列を追加する関数
// 同じ文字がないことが前提
// hash_table_checkで確認してから使用
int hash_table_add(HashNode *hash_table[], const char *buffer, int length, int total_characters) {
    int hash_index_number = hash_index(buffer);
    int i = 0;
    if(hash_table[hash_index_number] == NULL) {
        hash_table[hash_index_number] = (HashNode *)malloc(sizeof(HashNode));
        if (hash_table[hash_index_number] == NULL) {
            perror("Failed to malloc");
            return -1;
        }
        hash_table[hash_index_number]->word = (char *)malloc(sizeof(char) * (length + 1));
        if (hash_table[hash_index_number]->word == NULL) {
            perror("Failed to malloc");
            free(hash_table[hash_index_number]);
            return -1;
        }
        strncpy(hash_table[hash_index_number]->word, buffer, length);
        hash_table[hash_index_number]->word[length] = '\0';
        hash_table[hash_index_number]->count = 1;
        hash_table[hash_index_number]->length = length;
        hash_table[hash_index_number]->next = NULL;
        hash_table[hash_index_number]->longest = 0;
        hash_table[hash_index_number]->shortest = 0;
        hash_table[hash_index_number]->position[0] = total_characters;
        for (i = 1; i < 5; i++) {
            hash_table[hash_index_number]->position[i] = -1;
        }
    } else {
        HashNode *node = hash_table[hash_index_number];
        while (node->next != NULL) {
            node = node->next;
            i++;
        }
        node->next = (HashNode *)malloc(sizeof(HashNode));
        if (node->next == NULL) {
            perror("Failed to malloc");
            return -1;
        }
        node->next->word = (char *)malloc(sizeof(char) * (length + 1));
        if (node->next->word == NULL) {
            perror("Failed to malloc");
            free(node->next);
            return -1;
        }
        strncpy(node->next->word, buffer, length);
        node->next->word[length] = '\0';
        node->next->count = 1;
        node->next->length = length;
        node->next->next = NULL;
        node->next->longest = 0;
        node->next->shortest = 0;
        node->next->position[0] = total_characters;
        for (int j = 1; j < 5; j++) {
            node->next->position[j] = -1;
        }
    }
    return 0;
}

TrieNode* create_TrieNode(void) {
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    newNode->is_end_of_word = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}


void trie_insert(HashNode *hash_node[], TrieNode *root, char *word, int length) {
    TrieNode *current = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = create_TrieNode();
            if (current->children[index] == NULL) {
                perror("Failed to create TrieNode");
                return;
            }
        }
        current = current->children[index];
        if (i == length - 1) {
            int hashnode_index = hash_index(word);
            current->is_end_of_word = 1;
            HashNode *node = hash_node[hashnode_index];
            while (strcmp(node->word, word) != 0) {
                node = node->next;
            }
            for (int j = 0; j < 5; j++) {
                current->position[j] = node->position[j];
            }
        } else {
            current->is_end_of_word = 0;
            for (int j = 0; j < 5; j++) {
                current->position[j] = -1;
            }
        }
    }
    return;
}

// 深さ優先探索
// 該当する接頭辞がなかったら，NULLを返す
// 接頭辞の終端ノードを探す
TrieNode *trie_search_prefix(TrieNode *root, const char *prefix) {
    TrieNode *current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';
        if (current->children[index] == NULL) {
            return NULL;
        }
        current = current->children[index];
    }
    return current; 
}

// listの用意はやっておく
void trie_search_word(WordList *list, TrieNode *node, int depth, char *buffer) {
    if (list->count >= list->capacity) {
        list->capacity += 10;
        list->word = (char **)realloc(list->word, sizeof(char *) * list->capacity);
        list->position = (int **)realloc(list->position, sizeof(int *) * list->capacity);
        if (list->word == NULL || list->position == NULL) {
            perror("Failed to realloc for word array");
            free(list);
            return;
        }
    }
    if (node->is_end_of_word) {
        list->position[list->count] = (int *)malloc(sizeof(int) * 5);
        for (int i = 0; i < 5; i++) {
            list->position[list->count][i] = -1;
        }
        // バッファ終端処理
        buffer[depth] = '\0';
        list->word[list->count] = (char *)malloc(sizeof(char) * (depth + 1));
        if (list->word[list->count] == NULL) {
            perror("Failed to malloc for word array");
            free(list);
            return;
        }
        strncpy(list->word[list->count], buffer, depth);
        list->word[list->count][depth] = '\0';
        if (list->position[list->count] == NULL || list->word[list->count] == NULL) {
            perror("Failed to malloc for position array");
            free(list);
            return;
        }
        for (int i = 0; i < 5; i++) {
            list->position[list->count][i] = node->position[i];
        }
        list->count++;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            buffer[depth] = 'a' + i;
            trie_search_word(list, node->children[i], depth + 1, buffer);
        }
    }
    return;
}


void swap(HashNode **a, HashNode **b) {
    HashNode *temp = *a;
    *a = *b;
    *b = temp;
}
// heap_sizeは新ノード追加前のサイズ
/*
親ノードのインデックス: (i - 1) / 2
左の子ノードのインデックス: 2 * i + 1
右の子ノードのインデックス: 2 * i + 2
親の方が常に小さい
*/
void heap_add(HashNode *heap[], HashNode *node) {
    int index = 0;
    while (heap[index] != NULL) {
        index++;
    }
    heap[index] = node;
    while (1) {
        int parent_index = (index - 1) / 2;
        if (heap[parent_index]->count <= heap[index]->count) {
            break;
        }
        swap(&heap[parent_index], &heap[index]);
        index = parent_index;
    }
    return;
}


// targetに対応するノードを下にシフトダウン
void heap_shiftdown(HashNode *heap[], int n, int target) {
    int left_child_index = 2 * target + 1;
    int right_child_index = 2 * target + 2;
    if (right_child_index >= n) {
        return;
    }
    if (heap[target]->count >= heap[left_child_index]->count &&
        heap[target]->count >= heap[right_child_index]->count) {
        return;
    }
    if (heap[left_child_index]->count < heap[target]->count) {
        swap(&heap[target], &heap[left_child_index]);
        target = left_child_index;
        heap_shiftdown(heap, n, target);
    }
    if (heap[right_child_index]->count < heap[target]->count) {
        swap(&heap[target], &heap[right_child_index]);
        target = right_child_index;
        heap_shiftdown(heap, n, target);
    }
    
}

// 最小の要素を削除して，それのポインタを返す
// heap_sizeは関数後にデクリメント
HashNode *heap_remove(HashNode *heap[], int n) {
    swap(&heap[0], &heap[n - 1]);
    HashNode *removed_node = heap[n - 1];
    heap_shiftdown(heap, n - 1, 0);
    return removed_node;
}

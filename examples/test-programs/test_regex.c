/**
 * Test Case 13: Regular Expressions
 * Tests: regcomp, regexec, pattern matching, groups, POSIX regex
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_MATCHES 10
#define MAX_ERROR_MSG 100

// Print regex compilation error
void print_regex_error(int errcode, regex_t *compiled) {
    char error_msg[MAX_ERROR_MSG];
    regerror(errcode, compiled, error_msg, sizeof(error_msg));
    fprintf(stderr, "Regex error: %s\n", error_msg);
}

// Test basic pattern matching
void test_basic_matching() {
    printf("\n1. Basic Pattern Matching:\n");
    
    regex_t regex;
    int ret;
    const char *pattern = "hello";
    const char *text1 = "hello world";
    const char *text2 = "goodbye world";
    
    // Compile regex
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        print_regex_error(ret, &regex);
        return;
    }
    
    // Test match
    ret = regexec(&regex, text1, 0, NULL, 0);
    printf("'%s' matches '%s': %s\n", text1, pattern, 
           ret == 0 ? "YES" : "NO");
    
    ret = regexec(&regex, text2, 0, NULL, 0);
    printf("'%s' matches '%s': %s\n", text2, pattern,
           ret == 0 ? "YES" : "NO");
    
    regfree(&regex);
}

// Test case-insensitive matching
void test_case_insensitive() {
    printf("\n2. Case-Insensitive Matching:\n");
    
    regex_t regex;
    const char *pattern = "hello";
    const char *texts[] = {"Hello", "HELLO", "hello", "HeLLo"};
    
    regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE);
    
    for (int i = 0; i < 4; i++) {
        int ret = regexec(&regex, texts[i], 0, NULL, 0);
        printf("'%s' matches: %s\n", texts[i], ret == 0 ? "YES" : "NO");
    }
    
    regfree(&regex);
}

// Test character classes
void test_character_classes() {
    printf("\n3. Character Classes:\n");
    
    typedef struct {
        const char *pattern;
        const char *text;
        const char *description;
    } TestCase;
    
    TestCase tests[] = {
        {"[0-9]+", "abc123def", "Digits"},
        {"[a-z]+", "ABC123abc", "Lowercase letters"},
        {"[A-Z]+", "abc123ABC", "Uppercase letters"},
        {"[a-zA-Z]+", "123abc456DEF", "Letters"},
        {"[^0-9]+", "abc123def", "Non-digits"}
    };
    
    for (int i = 0; i < 5; i++) {
        regex_t regex;
        regmatch_t match;
        
        regcomp(&regex, tests[i].pattern, REG_EXTENDED);
        int ret = regexec(&regex, tests[i].text, 1, &match, 0);
        
        printf("%s - Pattern: '%s', Text: '%s' - ", 
               tests[i].description, tests[i].pattern, tests[i].text);
        
        if (ret == 0) {
            printf("Match at [%d, %d]: '", match.rm_so, match.rm_eo);
            for (int j = match.rm_so; j < match.rm_eo; j++) {
                printf("%c", tests[i].text[j]);
            }
            printf("'\n");
        } else {
            printf("No match\n");
        }
        
        regfree(&regex);
    }
}

// Test quantifiers
void test_quantifiers() {
    printf("\n4. Quantifiers (* + ? {n,m}):\n");
    
    typedef struct {
        const char *pattern;
        const char *text;
    } Test;
    
    Test tests[] = {
        {"a*b", "aaab"},        // * = 0 or more
        {"a+b", "aaab"},        // + = 1 or more
        {"a?b", "ab"},          // ? = 0 or 1
        {"a{3}b", "aaab"},      // {3} = exactly 3
        {"a{2,4}b", "aaab"},    // {2,4} = 2 to 4
        {"a{2,}b", "aaaaab"}    // {2,} = 2 or more
    };
    
    for (int i = 0; i < 6; i++) {
        regex_t regex;
        regcomp(&regex, tests[i].pattern, REG_EXTENDED);
        int ret = regexec(&regex, tests[i].text, 0, NULL, 0);
        printf("'%s' matches '%s': %s\n", 
               tests[i].text, tests[i].pattern, ret == 0 ? "YES" : "NO");
        regfree(&regex);
    }
}

// Test anchors
void test_anchors() {
    printf("\n5. Anchors (^ and $):\n");
    
    regex_t regex;
    
    // Start anchor ^
    regcomp(&regex, "^hello", REG_EXTENDED);
    printf("'^hello' matches 'hello world': %s\n",
           regexec(&regex, "hello world", 0, NULL, 0) == 0 ? "YES" : "NO");
    printf("'^hello' matches 'say hello': %s\n",
           regexec(&regex, "say hello", 0, NULL, 0) == 0 ? "YES" : "NO");
    regfree(&regex);
    
    // End anchor $
    regcomp(&regex, "world$", REG_EXTENDED);
    printf("'world$' matches 'hello world': %s\n",
           regexec(&regex, "hello world", 0, NULL, 0) == 0 ? "YES" : "NO");
    printf("'world$' matches 'world peace': %s\n",
           regexec(&regex, "world peace", 0, NULL, 0) == 0 ? "YES" : "NO");
    regfree(&regex);
}

// Test groups and captures
void test_groups() {
    printf("\n6. Groups and Captures:\n");
    
    regex_t regex;
    regmatch_t matches[MAX_MATCHES];
    const char *pattern = "([a-z]+)@([a-z]+)\\.([a-z]+)";
    const char *email = "user@example.com";
    
    regcomp(&regex, pattern, REG_EXTENDED);
    
    if (regexec(&regex, email, MAX_MATCHES, matches, 0) == 0) {
        printf("Email: %s\n", email);
        
        for (int i = 0; i < MAX_MATCHES && matches[i].rm_so != -1; i++) {
            int len = matches[i].rm_eo - matches[i].rm_so;
            char match[256];
            strncpy(match, email + matches[i].rm_so, len);
            match[len] = '\0';
            printf("  Group %d: '%s'\n", i, match);
        }
    }
    
    regfree(&regex);
}

// Test alternation
void test_alternation() {
    printf("\n7. Alternation (|):\n");
    
    regex_t regex;
    const char *pattern = "cat|dog|bird";
    const char *texts[] = {"I have a cat", "I have a dog", 
                           "I have a bird", "I have a fish"};
    
    regcomp(&regex, pattern, REG_EXTENDED);
    
    for (int i = 0; i < 4; i++) {
        int ret = regexec(&regex, texts[i], 0, NULL, 0);
        printf("'%s' matches: %s\n", texts[i], ret == 0 ? "YES" : "NO");
    }
    
    regfree(&regex);
}

// Test special characters
void test_special_characters() {
    printf("\n8. Special Characters:\n");
    
    typedef struct {
        const char *pattern;
        const char *text;
        const char *desc;
    } Test;
    
    Test tests[] = {
        {".", "a", "Any character"},
        {".+", "hello", "One or more any"},
        {"\\d", "5", "Digit (not in POSIX, using [0-9])"},
        {"[0-9]", "5", "Digit"},
        {"\\.", "test.txt", "Literal dot"},
        {"[[:digit:]]+", "123", "POSIX digit class"},
        {"[[:alpha:]]+", "abc", "POSIX alpha class"},
        {"[[:space:]]+", "   ", "POSIX space class"}
    };
    
    for (int i = 0; i < 8; i++) {
        regex_t regex;
        int ret = regcomp(&regex, tests[i].pattern, REG_EXTENDED);
        
        if (ret != 0) {
            printf("%s - Pattern '%s': COMPILE ERROR\n", 
                   tests[i].desc, tests[i].pattern);
            continue;
        }
        
        ret = regexec(&regex, tests[i].text, 0, NULL, 0);
        printf("%s - '%s' matches '%s': %s\n",
               tests[i].desc, tests[i].text, tests[i].pattern,
               ret == 0 ? "YES" : "NO");
        
        regfree(&regex);
    }
}

// Test find all matches
void test_find_all() {
    printf("\n9. Find All Matches:\n");
    
    regex_t regex;
    regmatch_t match;
    const char *pattern = "[0-9]+";
    const char *text = "There are 123 apples and 456 oranges";
    const char *p = text;
    
    regcomp(&regex, pattern, REG_EXTENDED);
    
    printf("Finding all numbers in: '%s'\n", text);
    
    while (regexec(&regex, p, 1, &match, 0) == 0) {
        int len = match.rm_eo - match.rm_so;
        char matched[256];
        strncpy(matched, p + match.rm_so, len);
        matched[len] = '\0';
        printf("  Found: '%s' at position %ld\n", 
               matched, (long)(p - text + match.rm_so));
        p += match.rm_eo;
    }
    
    regfree(&regex);
}

// Test validation patterns
void test_validation() {
    printf("\n10. Validation Patterns:\n");
    
    // Email validation (simplified)
    regex_t email_regex;
    const char *email_pattern = "^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    regcomp(&email_regex, email_pattern, REG_EXTENDED);
    
    const char *emails[] = {"user@example.com", "invalid.email", "test@test.co.uk"};
    printf("Email validation:\n");
    for (int i = 0; i < 3; i++) {
        int ret = regexec(&email_regex, emails[i], 0, NULL, 0);
        printf("  '%s': %s\n", emails[i], ret == 0 ? "VALID" : "INVALID");
    }
    regfree(&email_regex);
    
    // Phone number validation (US format)
    regex_t phone_regex;
    const char *phone_pattern = "^[0-9]{3}-[0-9]{3}-[0-9]{4}$";
    regcomp(&phone_regex, phone_pattern, REG_EXTENDED);
    
    const char *phones[] = {"123-456-7890", "1234567890", "123-45-6789"};
    printf("\nPhone validation (XXX-XXX-XXXX):\n");
    for (int i = 0; i < 3; i++) {
        int ret = regexec(&phone_regex, phones[i], 0, NULL, 0);
        printf("  '%s': %s\n", phones[i], ret == 0 ? "VALID" : "INVALID");
    }
    regfree(&phone_regex);
}

int main() {
    printf("=== Regular Expressions Test ===\n");
    
    test_basic_matching();
    test_case_insensitive();
    test_character_classes();
    test_quantifiers();
    test_anchors();
    test_groups();
    test_alternation();
    test_special_characters();
    test_find_all();
    test_validation();
    
    printf("\n=== Regular Expressions Test Complete ===\n");
    return 0;
}

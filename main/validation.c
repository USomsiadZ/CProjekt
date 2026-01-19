#include "validation.h"
#include "constants.h"
#include <string.h>
#include <stddef.h>

bool validate_author(const char* author) {
    if (author == NULL) {
        return false;
    }
    
    size_t len = strlen(author);
    
    if (len == 0) {
        return false;
    }
    
    if (len >= MAX_AUTHOR_LEN) {
        return false;
    }
    
    return true;
}

bool validate_content(const char* content) {
    if (content == NULL) {
        return false;
    }
    
    size_t len = strlen(content);
    
    if (len == 0) {
        return false;
    }
    // '\0'
    if (len >= MAX_CONTENT_LEN) {
        return false;
    }
    
    return true;
}

bool validate_category(const char* category) {
    if (category == NULL) {
        return false;
    }
    
    if (strcmp(category, CATEGORY_SPAM) == 0 ||
        strcmp(category, CATEGORY_HATE) == 0 ||
        strcmp(category, CATEGORY_PROFANITY) == 0 ||
        strcmp(category, CATEGORY_FAKE_NEWS) == 0 ||
        strcmp(category, CATEGORY_INAPPROPRIATE) == 0 ||
        strcmp(category, CATEGORY_OK) == 0) {
        return true;
    }
    
    return false;
}

bool validate_status(const char* status) {
    if (status == NULL) {
        return false;
    }
    
    if (strcmp(status, STATUS_TO_VERIFY) == 0 ||
        strcmp(status, STATUS_IN_ANALYSIS) == 0 ||
        strcmp(status, STATUS_APPROVED) == 0 ||
        strcmp(status, STATUS_DELETED) == 0) {
        return true;
    }
    
    return false;
}

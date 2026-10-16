# csv regression tests

from the repo root:

    cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -g tests/test_csv.c -o /tmp/test-contactbook-csv
    /tmp/test-contactbook-csv
    python3 -m unittest discover -s tests -p 'test_*.py' -v

all test data is temporary. the tests do not touch contact_book_data.csv.

newly saved fields are quoted, and embedded quotes are doubled. the reader keeps
empty fields and accepts existing unquoted four-column rows. malformed rows,
extra columns and fields longer than 99 bytes are skipped instead of shifted or
truncated. records stay one physical line, matching the app's line-based prompts;
multiline csv fields are not supported. old rows whose commas already made their
columns ambiguous cannot be repaired automatically.

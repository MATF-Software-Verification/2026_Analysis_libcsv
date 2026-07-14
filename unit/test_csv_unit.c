#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcsv/csv.h"

int field_count;
int row_count;


// Callback funkcija za svako polje
void field_cb(void *field, size_t len, void *data) {
    (void)data;

    char *str = (char *)field;

    if (str) {
        printf("FIELD: %.*s\n", (int)len, str);
        field_count++;
    } else {
        printf("FIELD: NULL\n");
    }
}


// Callback funkcija za kraj reda
void row_cb(int c, void *data) {
    (void)data;

    row_count++;

    if (c == -1)
        printf("ROW END: EOF\n");
    else
        printf("ROW END: %c\n", (char)c);
}


// Standardni parser test
int run_test(const char *csv_data) {

    struct csv_parser parser;

    size_t data_length = strlen(csv_data);


    field_count = 0;
    row_count = 0;


    printf("\n--------------------\n");
    printf("TEST INPUT:\n%s\n", csv_data);
    printf("--------------------\n");


    if (csv_init(&parser, CSV_APPEND_NULL) != 0) {
        fprintf(stderr, "Failed to initialize parser\n");
        return 0;
    }


    size_t parsed = csv_parse(
        &parser,
        csv_data,
        data_length,
        field_cb,
        row_cb,
        NULL
    );


    if (parsed != data_length) {

        fprintf(stderr,
                "Parse error: %s\n",
                csv_strerror(csv_error(&parser)));

        csv_free(&parser);
        return 0;
    }


    csv_fini(
        &parser,
        field_cb,
        row_cb,
        NULL
    );


    csv_free(&parser);


    printf("Parsed characters: %zu\n", parsed);
    printf("Fields: %d\n", field_count);
    printf("Rows: %d\n", row_count);


    return 1;
}



// Test promene delimitera
int custom_delimiter_test()
{
    struct csv_parser parser;

    char data[] =
        "name;age;city\n"
        "Ana;25;Belgrade";


    printf("\nCUSTOM DELIMITER TEST\n");


    csv_init(&parser, CSV_APPEND_NULL);


    csv_set_delim(&parser, ';');


    csv_parse(
        &parser,
        data,
        strlen(data),
        field_cb,
        row_cb,
        NULL
    );


    csv_fini(
        &parser,
        field_cb,
        row_cb,
        NULL
    );


    csv_free(&parser);


    return 1;
}



// Test velikog polja
int large_field_test()
{
    char big_field[500];

    memset(big_field, 'A', sizeof(big_field)-1);
    big_field[sizeof(big_field)-1] = '\0';


    printf("\nLARGE FIELD TEST\n");


    return run_test(big_field);
}



// Test strict greske
int strict_error_test()
{
    struct csv_parser parser;


    char bad_csv[] =
        "abc\"def,123";


    printf("\nSTRICT ERROR TEST\n");


    csv_init(
        &parser,
        CSV_APPEND_NULL | CSV_STRICT
    );


    size_t parsed =
        csv_parse(
            &parser,
            bad_csv,
            strlen(bad_csv),
            field_cb,
            row_cb,
            NULL
        );


    if (parsed != strlen(bad_csv)) {

        printf("Expected parsing error:\n");
        printf("%s\n",
            csv_strerror(csv_error(&parser)));

        csv_free(&parser);

        return 1;
    }


    csv_free(&parser);

    return 0;
}



// Test csv_write funkcije
int write_test()
{
    char output[100];


    printf("\nCSV WRITE TEST\n");

    
    int written =
        csv_write(
            output,
            sizeof(output),
            "hello,world",
            strlen("hello,world")
        );
      output[written] = '\0';


    printf("Written chars: %d\n", written);
    printf("Output: %s\n", output);


    return written > 0;
}

// Test promene quote karaktera
int quote_test()
{
    struct csv_parser parser;

    char data[] =
        "'hello','world'";


    printf("\nQUOTE TEST\n");


    csv_init(
        &parser,
        CSV_APPEND_NULL
    );


    csv_set_quote(
        &parser,
        '\''
    );


    csv_parse(
        &parser,
        data,
        strlen(data),
        field_cb,
        row_cb,
        NULL
    );


    csv_fini(
        &parser,
        field_cb,
        row_cb,
        NULL
    );


    csv_free(&parser);


    return 1;
}



// Test getter funkcija
int getter_test()
{
    struct csv_parser parser;


    printf("\nGETTER TEST\n");


    csv_init(
        &parser,
        CSV_APPEND_NULL
    );


    printf("Delimiter: %c\n",
           csv_get_delim(&parser));


    printf("Quote: %c\n",
           csv_get_quote(&parser));


    printf("Options: %d\n",
           csv_get_opts(&parser));


    printf("Buffer size: %zu\n",
           csv_get_buffer_size(&parser));


    csv_free(&parser);


    return 1;
}


int main() {


    const char *test_cases[] = {


        // 1. Osnovni CSV
        "name,age,city\n"
        "Ana,25,Belgrade\n"
        "Marko,30,Nis",



        // 2. Polje sa zarezom
        "\"New York, USA\",30,NY",



        // 3. Prazna polja
        "a,,c\n"
        "1,,3",



        // 4. Escape navodnika
        "\"He said \"\"hello\"\"\",123",



        // 5. CRLF
        "a,b,c\r\n"
        "1,2,3",



        // 6. Jedno polje
        "single_value",



        // 7. Prazan ulaz
        "",



        // 8. Razmaci
        "  Ana  , 25 , Beograd ",



        // 9. Prazni redovi
        "a,b\n\nc,d",



        // 10. Newline unutar navodnika
        "\"hello\nworld\",123",
       
       
        // 11. Razmak nakon zatvorenog navodnika
        "\"hello\"  ,123",


        // 12. Veliki broj kolona
        "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t",



        NULL
    };



    int passed = 0;
    int total = 0;



    for (int i = 0; test_cases[i] != NULL; i++) {

        total++;


        if (run_test(test_cases[i])) {

            printf("TEST %d PASSED\n", i+1);
            passed++;

        } else {

            printf("TEST %d FAILED\n", i+1);

        }
    }



    printf("\n--------------------\n");
    printf("ADDITIONAL TESTS\n");
    printf("--------------------\n");


    total++;
    if(custom_delimiter_test()) {
        printf("CUSTOM DELIMITER PASSED\n");
        passed++;
    }


    total++;
    if(large_field_test()) {
        printf("LARGE FIELD PASSED\n");
        passed++;
    }


    total++;
    if(strict_error_test()) {
        printf("STRICT ERROR PASSED\n");
        passed++;
    }


    total++;
    if(write_test()) {
        printf("WRITE TEST PASSED\n");
        passed++;
    }

    total++;
    if(quote_test()) {
        printf("QUOTE TEST PASSED\n");
        passed++;
    }


    total++;
    if(getter_test()) {
        printf("GETTER TEST PASSED\n");
        passed++;
    }

    printf("\n---------------------\n");
    printf("UNIT TEST SUMMARY\n");
    printf("Passed: %d/%d\n", passed, total);



    if (passed == total) {

        printf("ALL UNIT TESTS PASSED\n");
        return 0;

    }
    else {

        printf("SOME UNIT TESTS FAILED\n");
        return 1;

    }
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "json.h"

#define QUOTE(...) #__VA_ARGS__

unsigned char *pJson = QUOTE(
        {
            "firstName": "John",
            "lastName" : "Smith",
            "isAlive" : true,
            "age" : 27,
            "address" : {
            "streetAddress": "21 2nd Street",
                "city" : "New York",
                "state" : "NY",
                "postalCode" : "10021-3100"
        },
            "phoneNumbers": [
            {
                "type": "office",
                    "number" : "646 555-4567"
            },
            {
                "type": "mobile",
                "number" : "123 456-7890"
            }
            ],
                "children": [],
                    "spouse" : null
        }
        );


static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
                printf(" ");
        }
}

static void process_value(json_value* value, int depth);

static void process_object(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                print_depth_shift(depth);
                printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
                process_value(value->u.object.values[x].value, depth+1);
        }
}

static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        printf("array\n");
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth);
        }
}

static void process_value(json_value* value, int depth)
{
        
        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
                        printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
                        printf("int: %llu: \n", value->u.integer);
                        break;
                case json_double:
                        printf("double: %f\n", value->u.dbl);
                        break;
                case json_string:
                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}

int main(int argc, char** argv)
{
        json_char* json;
        json_value* value;
        size_t len = 0;


        json = (json_char*)pJson;
        len = strlen(pJson);

        value = json_parse(json, len);

        process_value(value, 0);

        json_value_free(value);
        return 0;
}

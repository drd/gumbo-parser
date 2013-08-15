#include <node.h>
#include <v8.h>

#include "../src/gumbo.h"

using namespace v8;

void create_parse_tree(Local<Object> tree, GumboNode* root);


Handle<Value> Method(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 1) {
	ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	return scope.Close(Undefined());
    }

    if (!args[0]->IsString()) {
	ThrowException(Exception::TypeError(String::New("Wrong arguments")));
	return scope.Close(Undefined());
    }

    String::Utf8Value str(args[0]->ToString());
    char* c_str = *str;

    GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions,
						   c_str,
						   args[0]->ToString()->Utf8Length());

    Local<Object> tree = Object::New();

    create_parse_tree(tree, output->root);

    return scope.Close(tree);
}

void create_parse_tree(Local<Object> tree, GumboNode* root) {
    GumboElement element = root->v.element;
    tree->Set(String::NewSymbol("tag"),
	      String::New(element.original_tag.data,
			  element.original_tag.length));

    String::Utf8Value str(tree->Get(String::NewSymbol("tag")));
    char *c_str = *str;
    printf("Tag: %s\n", c_str);

    tree->Set(String::NewSymbol("length"),
	      Number::New(element.children.length));

    Local<Array> children = Array::New();
    tree->Set(String::NewSymbol("children"),
	      children);

    printf("node type: %d\n", root->type);
    if (root->type != GUMBO_NODE_ELEMENT) {
	printf("not an element, skipping children.\n");
	return;
    }
    GumboVector* root_children = &element.children;
    printf("create_parse_tree, %d\n", root_children->length);
    for (uint i=0; i < element.children.length; i++) {
	printf("about to make child %d\n", i);
	Local<Object> child = Object::New();
	children->Set(Number::New(i), child);
	GumboNode *root_child = (GumboNode* )root_children->data[i];
	create_parse_tree(child, root_child);
    }
}

void init(Handle<Object> exports) {
    exports->Set(String::NewSymbol("parse"),
		 FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(gumbo, init)

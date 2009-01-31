#include <stdio.h>
#include <stdlib.h>

#if 1
#include "test.h"

/* we need a different exit status to denote a test error */
#define TEST_ERROR 2

Test test;

int32_t TestRecipricant::func (const Args &args, Args &out)
{
        if (args.size () < 1) {
                out.push_back ("not enough arguements");
        } else if (args.size () == 1) {
                /* one special case if there are no args the test is false */
                return PLUGIN_FAILURE;
        } else {
                return test.test (args, out);
        }
        return TEST_ERROR;
}
#else

#include <string>
#include <list>

typedef std::string Arg;
typedef std::list<Arg> Args;

#endif

static bool isInt (const std::string &n)
{
        char *endptr = NULL;
        strtol (n.c_str (), &endptr, 0);
        if (*endptr == '\0') {
                return true;
        } else {
                return false;
        }
}

enum expr_t
{
        e_invalid,
        e_str,
        e_int,
        e_expr,
        e_eunary,
        e_ebinary,
        e_sunary,
        e_sbinary,
        e_nbinary,
};

struct node
{
        expr_t type;
        std::string val;

        node *left;
        node *right;
};

static node *parse_expr_obinary (const Args &args, Args::const_iterator &i);
static node *parse_expr_abinary (const Args &args, Args::const_iterator &i);
static node *parse_expr_unary (const Args &args, Args::const_iterator &i);
static node *parse_str_binary (const Args &args, Args::const_iterator &i);
static node *parse_num_binary (const Args &args, Args::const_iterator &i);
static node *parse_str_unary (const Args &args, Args::const_iterator &i);
static node *parse_basic (const Args &args, Args::const_iterator &i);

static node *make_node (expr_t type, const std::string &val)
{
        node *n = new node;
        n->type = type;
        n->val = val;

        n->right = NULL;
        n->left = NULL;
        return n;
}

static node *parse_expr (const Args &args, Args::const_iterator &i)
{
        return parse_expr_obinary (args, i);
}

static node *parse_expr_obinary (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        node *left = parse_expr_abinary (args, i);

        if (*i == "-o" ) {
                node *ret = make_node (e_ebinary, *i);
                i ++;
                ret->left = left;
                ret->right = parse_expr_obinary (args, i);
                return ret;
        }
        return left;
}
        
static node *parse_expr_abinary (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        node *left = parse_expr_unary (args, i);

        if (*i == "-a" ) {
                node *ret = make_node (e_ebinary, *i);
                i ++;
                ret->left = left;
                ret->right = parse_expr_abinary (args, i);
                return ret;
        }
        return left;
}
        
static node *parse_expr_unary (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        if (*i == "!") {
                node *ret = make_node (e_eunary, *i);
                i ++;
                ret->right = parse_expr_unary (args, i);
                return ret;
        }
        return parse_str_binary (args, i);
}
        
static node *parse_str_binary (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        node *left = parse_num_binary (args, i);

        if (*i == "==" || *i == "!=" ) {
                node *ret = make_node (e_sbinary, *i);
                i ++;
                ret->left = left;
                ret->right = parse_str_binary (args, i);
                return ret;
        }
        return left;
}
        
static node *parse_num_binary (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        node *left = parse_str_unary (args, i);

        if (*i == "-eq" || *i == "-ge" || *i == "-gt" 
         || *i == "-le" || *i == "-lt" || *i == "-ne" ) {
                node *ret = make_node (e_nbinary, *i);
                i ++;
                ret->left = left;
                ret->right = parse_num_binary (args, i);
                return ret;
        }
        return left;
}
        
static node *parse_str_unary (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        if (*i == "-n" || *i == "-z") {
                node *ret = make_node (e_sunary, *i);
                i ++;
                ret->right = parse_str_unary (args, i);
                return ret;
        }
        return parse_basic (args, i);
}

static node *parse_basic (const Args &args, Args::const_iterator &i)
{
        if (i == args.end ()) {
                return NULL;
        }

        if (*i == "(") {
                i ++;
                node *ret = parse_expr_obinary (args, i);

                if (i == args.end ()) {
                        delete ret;
                        return NULL;
                }
                i ++; 
                return ret;
        }
        node *ret;
        if (isInt (*i)) {
                ret = make_node (e_int, *i);
        } else {
                ret = make_node (e_str, *i);
        }
        i ++;
        return ret; 
}

expr_t check_expr (node *n, Args &error)
{
        if (n == NULL) return e_invalid;

        if (n->type == e_ebinary) {
                if (check_expr (n->left, error) != e_expr) {
                        error.push_back ("expected expr left of " + n->val);
                        return e_invalid;
                }
                if (check_expr (n->right, error) != e_expr) {
                        error.push_back ("expected expr right of " + n->val);
                        return e_invalid;
                }
                return e_expr;
        } else if (n->type == e_eunary) {
                if (check_expr (n->right, error) != e_expr) {
                        error.push_back ("expected expr right of " + n->val);
                        return e_invalid;
                }
                return e_expr;
        } else if (n->type == e_sbinary) {
                expr_t tmp = check_expr (n->left, error);
                if (tmp != e_str && tmp != e_int) {
                        error.push_back ("expected string left of " + n->val);
                        return e_invalid;
                }
                tmp = check_expr (n->right, error);
                if (tmp != e_str && tmp != e_int) {
                        error.push_back ("expected string right of " + n->val);
                        return e_invalid;
                }
                return e_expr;
        } else if (n->type == e_nbinary) {
                if (check_expr (n->left, error) != e_int) {
                        error.push_back ("expected int left of " + n->val);
                        return e_invalid;
                }
                if (check_expr (n->left, error) != e_int) {
                        error.push_back ("expected int right of " + n->val);
                        return e_invalid;
                }
                return e_expr;
        } else if (n->type == e_sunary) {
                expr_t tmp = check_expr (n->right, error);
                if (tmp != e_str && tmp != e_int) {
                        error.push_back ("expected str right of " + n->val);
                        return e_invalid;
                }
                return e_expr;
        } else {
                return n->type;
        }
}

bool eval_expr (node *n) {
        if (n->type == e_ebinary) {
                if (n->val == "-a") {
                        return eval_expr (n->left) && eval_expr (n->right);
                } else if (n->val == "-o") {
                        return eval_expr (n->left) || eval_expr (n->right);
                } else {
                        printf ("eval_expr ebinary error\n");
                        return false;
                }
        } else if (n->type == e_eunary) {
                if (n->val == "!") {
                        return ! eval_expr (n->right);
                } else {
                        printf ("eval_expr eunary error\n");
                        return false;
                }
        } else if (n->type == e_sbinary) {
                if (n->val == "==") {
                        return n->left->val == n->right->val;
                } else if (n->val == "!=") {
                        return n->left->val != n->right->val;
                } else {
                        printf ("eval_expr sbinary error\n");
                        return false;
                }
        } else if (n->type == e_nbinary) {
                int32_t left = strtol (n->left->val.c_str (), NULL, 0);
                int32_t right = strtol (n->right->val.c_str (), NULL, 0);
                if (n->val == "-eq") return left == right;
                else if (n->val == "-ge") return left >= right;
                else if (n->val == "-gt") return left > right;
                else if (n->val == "-le") return left <= right;
                else if (n->val == "-lt") return left < right;
                else if (n->val == "-ne") return left != right;
                else {
                        printf ("eval_expr nbinary error\n");
                        return false;
                }
        } else if (n->type == e_sunary) {
                if (n->val == "-n") {
                        return n->right->val.length ();
                } else if (n->val == "-z") {
                        return ! n->right->val.length ();
                } else {
                        printf ("eval_expr sunary error\n");
                        return false;
                }
        } else if (n->type == e_str || n->type == e_int) {
                return true;
        } else {
                printf ("eval_expr total failure\n");
                return false;
        }
}

void show_node (node *n)
{
        if (n == NULL) return;

        printf ("(");
        if (n->type == e_sunary || n->type == e_eunary) {
                printf ("%s", n->val.c_str ());
                show_node (n->right);
        } else {
                show_node (n->left);
                printf ("%s", n->val.c_str ());
                show_node (n->right);
        }
        printf (")");
}

void delete_node (node *n)
{
        if (n == NULL) return;

        delete_node (n->left);
        delete_node (n->right);

        delete n;
}

#if 0
/* for debugging purposes */
int main (int argc, char* *argv)
{
        Args test;

        for (int i = 1; i < argc; i ++) {
                test.push_back (argv[i]);
        }

        Args::const_iterator i = test.begin ();
        node *n = parse_expr (test, i);
        show_node (n);
        printf ("\n");

        Args error;

        if (check_expr (n, error) == e_invalid) {
                for (i = error.begin (); i != error.end (); ++ i) {
                        printf ("%s\n", (*i).c_str ());
                }
        } else {
                printf ("%d\n", eval_expr (n));
        }

        return 0;
}
#else
/**
 * Test is a bit different, the exit status depends on the result of the
 * expression
 */
int32_t Test::test (const Args &args, Args &out)
{
        Args::const_iterator i = args.begin ();
        i ++;
        node *n = parse_expr (args, i);

        int32_t ret;

        if (check_expr (n, out) == e_invalid) {
                out.push_front ("invalid expression");        
                return TEST_ERROR;
        } else {
                if (eval_expr (n)) {
                        ret = PLUGIN_SUCCESS;
                } else {
                        ret = PLUGIN_FAILURE;
                }
        }
        delete_node (n);
        return ret;
}

void Test::i_load (uint32_t pluginId, Client *client)
{
        _name = "test";
        _pluginId = pluginId;
        _client = client;

        _rid = _client->addRecipricant (_pluginId, new TestRecipricant ());
}

void Test::i_unload ()
{
        _client->deleteRecipricant (_pluginId, _rid);
}

const std::string &Test::i_name ()
{
        return _name;
}

void load (uint32_t id, Client *_client)
{
        test.i_load (id, _client);
}

void unload ()
{
        test.i_unload ();
}

const std::string &name ()
{
        return test.i_name ();
}
#endif


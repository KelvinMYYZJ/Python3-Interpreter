#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Any_operator.h"
#include "int2048.h"
typedef sjtu::int2048 num_type;
using std::string;
#include "Python3BaseVisitor.h"
#include <map>
typedef std::map<string, antlrcpp::Any> Scope;

static Scope val_map;
static std::stack<Scope> scope_stack;
class BreakSignal {
private:
    bool val;

public:
    BreakSignal() { }
};
class ContinueSignal {
private:
    bool val;

public:
    ContinueSignal() { }
};
class ReturnVal {
private:
    antlrcpp::Any val;

public:
    ReturnVal()
        : val(NULL)
    {
    }
    ReturnVal(const ReturnVal& other)
        : val(other.val)
    {
    }
    ReturnVal(antlrcpp::Any other_val)
        : val(other_val)
    {
    }
    antlrcpp::Any Get_Val()
    {
        return val;
    }
};
class ValName {
private:
    string name;

public:
    ValName() {};
    ValName(const string& other)
        : name(other) {};
    ValName(const ValName& other)
        : name(other.name) {};
    operator string() const
    {
        return name;
    }
    antlrcpp::Any Get_Val()
    {
        if (!scope_stack.empty() && scope_stack.top().find(name) != scope_stack.top().end())
            return scope_stack.top()[name];
        if (val_map.find(name) != val_map.end())
            return val_map[name];
        throw((val_map.size()));
    }
    ValName& operator=(const ValName& other)
    {
        name = other.name;
        return *this;
    }
};
class FunctionCtx {
public:
    Python3Parser::FuncdefContext* ctx;
    Scope default_scope;
    FunctionCtx() {};
    FunctionCtx(Python3Parser::FuncdefContext* other_ctx, Scope other_default_scope)
        : ctx(other_ctx)
        , default_scope(other_default_scope)
    {
    }
    FunctionCtx& operator=(const FunctionCtx& other)
    {
        ctx = other.ctx;
        default_scope = other.default_scope;
        return *this;
    }
    Python3Parser::FuncdefContext* Get_Ctx() const
    {
        return ctx;
    }
    Scope Get_Default_Scope() const
    {
        return default_scope;
    }
    // antlrcpp::Any Visit(std::vector<class Python3Parser::ArgumentContext*> argu_list)
    // {
    //     Scope temp_scope = default_scope;
    //     // todo : init
    //     scope_stack.push(temp_scope);
    //     auto ans = NULL;
    //     // todo : execute
    //     scope_stack.pop();
    //     return ans;
    // }
};

static void Get_Val(antlrcpp::Any& obj)
{
    if (obj.is<ValName>())
        obj = obj.as<ValName>().Get_Val();
    return;
}

static antlrcpp::Any Get_Val_Of(antlrcpp::Any obj)
{
    if (obj.is<ValName>())
        return obj.as<ValName>().Get_Val();
    return obj;
}

static void Edit_Val(const antlrcpp::Any& index, const antlrcpp::Any& value)
{
    if (!scope_stack.empty() && scope_stack.top().find(string(index.as<ValName>())) != scope_stack.top().end()) // found original local variable
	{
        scope_stack.top()[string(index.as<ValName>())] = value;
		return;
	}
	if(val_map.find(string(index.as<ValName>())) != val_map.end())
	{
        val_map[string(index.as<ValName>())] = value;
		return;
	}
	if (!scope_stack.empty())
	{
        scope_stack.top()[string(index.as<ValName>())] = value;
		return;
	}
	{
		//if (string(index.as<ValName>()) != "Seed" && string(index.as<ValName>()) != "jjjj")
			//std::cerr<<"creating new var:"<<string(index.as<ValName>())<<std::endl;
        val_map[string(index.as<ValName>())] = value;
	}
    return;
}
static bool Get_Bool(antlrcpp::Any obj) // turn ANY Any to bool
{
    Get_Val(obj);
    if (obj.is<bool>())
        return obj.as<bool>();
    if (obj.is<num_type>())
        return !obj.as<num_type>().Is_Zero();
    if (obj.is<double>())
        return obj.as<double>() != double(0);
    if (obj.is<string>())
        return obj.as<string>().size();
    if (obj.isNull())
        return false;
    throw("INVAILD CONDITION");
    return false;
}

class EvalVisitor : public Python3BaseVisitor {

    virtual antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext* ctx) override
    {
        if (!ctx->trailer())
            return visitAtom(ctx->atom());
        string function_name = ctx->atom()->getText();
        if (function_name == "print") {
            if (!(ctx->trailer()->arglist())) {
                std::cout << std::endl;
                return 0;
            }
            auto argu_list = ctx->trailer()->arglist()->argument();
            for (auto argu_iter = argu_list.begin(); argu_iter != argu_list.end();
                 argu_iter++) {
                if (argu_iter != argu_list.begin())
                    std::cout << ' ';
                auto ans = visitArgument(*argu_iter);
                Get_Val(ans);
                if (ans.is<num_type>())
                    std::cout << ans.as<num_type>();
                if (ans.is<string>())
                    std::cout << ans.as<string>();
                if (ans.is<bool>()) {
                    if (ans.as<bool>())
                        std::cout << "True";
                    else
                        std::cout << "False";
                }
                if (ans.isNull())
                    std::cout << "None";
            }
            std::cout << std::endl;
            return 0;
        }
        if (val_map.find(function_name) != val_map.end()) {
            //std::cerr << "visiting func:" << function_name << std::endl;
            // if (function_name == "random")
            // 	std::cout<<1;
            if (ctx->trailer()->arglist())
                return visitFunc(val_map[function_name].as<FunctionCtx>(), ctx->trailer()->arglist()->argument());
            return visitFunc(val_map[function_name].as<FunctionCtx>());
        }
        throw("UNDEFINED FUNCTION");
        return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitArgument(Python3Parser::ArgumentContext* ctx) override
    {
        auto test_list = ctx->test();
        if (test_list.size() == 1)
            return visitTest(*(ctx->test().begin()));
        return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitAtom(Python3Parser::AtomContext* ctx) override
    {
        if (!ctx->STRING().empty()) {
            string ans;
            auto string_list_to_treat = ctx->STRING();
            for (auto& temp_ctx : string_list_to_treat) {
                string temp_string = temp_ctx->getText();
                temp_string = temp_string.substr(1, temp_string.length() - 2);
                // std::cout<<x;
                ans.append(temp_string);
            }
            return ans;
        }
        if (ctx->NUMBER()) {
            num_type ans = ctx->NUMBER()->getText();
            return ans;
        }
        if (ctx->TRUE()) {
            return true;
        }
        if (ctx->FALSE()) {
            return false;
        }
        if (ctx->NONE()) {
            return antlrcpp::Any();
        }
        if (ctx->NAME()) {
            string val_name = ctx->NAME()->getText();
            return ValName(val_name);
        }
        if (ctx->test()) {
            return visitTest(ctx->test());
        }
        //std::cerr << ctx->getText();
        throw("UB");
    }

    virtual antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext* ctx) override
    {
        auto test_list = ctx->testlist();
        if (test_list.size() == 1)
            return visitChildren(ctx);
        if (!ctx->ASSIGN().empty()) {
            auto now = test_list.rbegin();
            antlrcpp::Any ans = visitTestlist(*now);
            Get_Val(ans);
            for (now++; now != test_list.rend(); now++) {
                antlrcpp::Any now_stmt = visitTestlist(*now);
                if (now_stmt.is<ValName>())
                    Edit_Val(now_stmt, ans);
                else
                    throw("fuck exprstmt");
            }
            return ans;
        }
        if (!ctx->augassign()->isEmpty()) {
            if (test_list.size() > 2)
                throw("INVAILD ?=");
            auto temp_augassign = ctx->augassign();
            antlrcpp::Any lhs = visitTestlist(*test_list.begin());
            antlrcpp::Any temp_ans = lhs;
            Get_Val(temp_ans);
            antlrcpp::Any rhs = visitTestlist(*test_list.rbegin());
            Get_Val(rhs);
            if (temp_augassign->ADD_ASSIGN()) // +=
                temp_ans += rhs;
            if (temp_augassign->SUB_ASSIGN()) // -=
                temp_ans -= rhs;
            if (temp_augassign->MULT_ASSIGN()) // *=
                temp_ans *= rhs;
            if (temp_augassign->DIV_ASSIGN()) // /=
                temp_ans /= rhs;
            if (temp_augassign->IDIV_ASSIGN()) // //=
                temp_ans <<= rhs;
            if (temp_augassign->MOD_ASSIGN()) // %=
                temp_ans %= rhs;
            Edit_Val(lhs, temp_ans);
            return temp_ans;
        }
        throw("INVAILD expr_stmt");
        return 0;
    }
    virtual antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext* ctx) override
    {
        auto term_list = ctx->term();
        auto op_list = ctx->addorsub_op();
        if (term_list.size() == 1)
            return visitChildren(ctx);
        auto term_iter = term_list.begin();
        auto op_iter = op_list.begin();
        antlrcpp::Any ans = visitTerm(*term_iter);
        Get_Val(ans);
        for (term_iter++; term_iter != term_list.end(); term_iter++, op_iter++) {
            auto temp_ans = visitTerm(*term_iter);
            Get_Val(temp_ans);
            if ((*op_iter)->ADD())
                ans += temp_ans;
            if ((*op_iter)->MINUS())
                ans -= temp_ans;
        }
        return ans;
    }
    virtual antlrcpp::Any visitTerm(Python3Parser::TermContext* ctx) override
    {
        //std::cerr << ctx->getText() << std::endl;
        auto factor_list = ctx->factor();
        auto op_list = ctx->muldivmod_op();
        if (ctx->factor().size() == 1)
            return visitChildren(ctx);
        auto factor_iter = factor_list.begin();
        auto op_iter = op_list.begin();
        auto ans = visitFactor(*factor_iter);
        Get_Val(ans);
        for (factor_iter++; factor_iter != factor_list.end(); factor_iter++, op_iter++) {
            antlrcpp::Any temp_ans = visitFactor(*factor_iter);
            Get_Val(temp_ans);
            if ((*op_iter)->STAR())
                ans *= temp_ans;
            if ((*op_iter)->DIV())
                ans /= temp_ans;
            if ((*op_iter)->IDIV())
                ans <<= temp_ans;
            if ((*op_iter)->MOD())
                ans %= temp_ans;
        }
        return ans;
    }
    virtual antlrcpp::Any visitFactor(Python3Parser::FactorContext* ctx) override
    {
        if (ctx->MINUS())
            return -Get_Val_Of(visitChildren(ctx));
        return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitComparison(Python3Parser::ComparisonContext* ctx) override
    {
        auto arith_list = ctx->arith_expr();
        if (arith_list.size() == 1)
            return visitChildren(ctx);
        auto op_list = ctx->comp_op();
        auto arith_iter = arith_list.begin();
        auto op_iter = op_list.begin();
        antlrcpp::Any lst_val = visitArith_expr(*arith_iter);
        Get_Val(lst_val);
        for (arith_iter++; arith_iter != arith_list.end(); arith_iter++, op_iter++) {
            auto now_val = visitArith_expr(*arith_iter);
            Get_Val(now_val);
            if ((*op_iter)->LESS_THAN()) // "<"
                if (!(lst_val < now_val))
                    return false;
            if ((*op_iter)->GREATER_THAN()) // ">"
                if (!(lst_val > now_val))
                    return false;
            if ((*op_iter)->EQUALS()) // "=="
                if (!(lst_val == now_val))
                    return false;
            if ((*op_iter)->GT_EQ()) // ">="
                if (!(lst_val >= now_val))
                    return false;
            if ((*op_iter)->LT_EQ()) // "<="
                if (!(lst_val <= now_val))
                    return false;
            if ((*op_iter)->NOT_EQ_2()) // "!="
                if (!(lst_val != now_val))
                    return false;
            lst_val = now_val;
        }
        return true;
    }
    virtual antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext* ctx) override
    {
        auto test_list = ctx->test();
        auto test_iter = test_list.begin();
        auto suite_list = ctx->suite();
        auto suite_iter = suite_list.begin();
        auto temp_string = ctx->getText();
        //std::cerr << "if_stmt:" << temp_string << std::endl;
        bool flag = false;
        for (; test_iter != test_list.end(); test_iter++, suite_iter++) {
            antlrcpp::Any test_val = visitTest(*test_iter);
            Get_Val(test_val);
            if (Get_Bool(test_val)) {
                //std::cerr << "True" << std::endl;
                //std::cerr << "executing:" << (*suite_iter)->getText() << std::endl;
                return visitSuite(*suite_iter);
            }
        }
        if (!flag && ctx->ELSE())
            return visitSuite(*suite_list.rbegin());
        return 0;
    }

    virtual antlrcpp::Any visitOr_test(Python3Parser::Or_testContext* ctx) override
    {
        if (ctx->and_test().size() == 1)
            return visitChildren(ctx);
        auto test_list = ctx->and_test();
        for (auto test_iter = test_list.begin(); test_iter != test_list.end(); test_iter++)
            if (Get_Bool(visitAnd_test(*test_iter)))
                return true;
        return false;
    }

    virtual antlrcpp::Any visitAnd_test(Python3Parser::And_testContext* ctx) override
    {
        if (ctx->not_test().size() == 1)
            return visitChildren(ctx);
        auto test_list = ctx->not_test();
        for (auto test_iter = test_list.begin(); test_iter != test_list.end(); test_iter++)
            if (!Get_Bool(visitNot_test(*test_iter)))
                return false;
        return true;
    }

    virtual antlrcpp::Any visitNot_test(Python3Parser::Not_testContext* ctx) override
    {
        if (ctx->NOT())
            // {
            // 	// bool temp = !(Get_Bool(visitNot_test(ctx->not_test())));
            // 	bool temp = !(Get_Bool(visitChildren(ctx)));
            // 	//std::cerr<<temp<<std::endl;
            // 	return temp;
            // }
            return !(Get_Bool(visitNot_test(ctx->not_test())));
        else
            return visitChildren(ctx);
    }
    virtual antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext* ctx) override
    {
        auto test_ctx = ctx->test();
        auto suite_ctx = ctx->suite();
        while (Get_Bool(visitTest(test_ctx))) {
            auto ans = visitSuite(suite_ctx);
            if (ans.is<BreakSignal>())
                return 0;
            if (ans.is<ReturnVal>())
                return ans;
        }
        return 0;
    }
    virtual antlrcpp::Any visitSuite(Python3Parser::SuiteContext* ctx) override
    {
        if (ctx->simple_stmt())
            return visitSimple_stmt(ctx->simple_stmt());
        auto stmt_list = ctx->stmt();
        for (auto stmt_iter = stmt_list.begin(); stmt_iter != stmt_list.end(); stmt_iter++) {
            auto ans = visitStmt(*stmt_iter);
            if (ans.is<BreakSignal>())
                return ans;
            if (ans.is<ContinueSignal>())
                return ans;
            if (ans.is<ReturnVal>())
                return ans;
        }
        return 0;
        // return visitChildren(ctx);
    }

    virtual antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext* ctx) override
    {
        antlrcpp::Any x = BreakSignal();
        if (x.is<bool>())
            return false;
        return x;
    }

    virtual antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext* ctx) override
    {
        return ContinueSignal();
    }

    virtual antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext* ctx) override
    {
        // auto x = visitSmall_stmt(ctx->small_stmt());
        // // auto x = visitChildren(ctx);
        // if (x.is<BreakSignal>())
        // 	std::cout<<"visitSimple_stmt   ";
        // return x;
        return visitSmall_stmt(ctx->small_stmt());
    }

    virtual antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext* ctx) override
    {
        ValName function_name = ctx->NAME()->getText();
        Scope temp_default_scope;
        auto targs_list = ctx->parameters()->typedargslist();
        if (!targs_list) {
            Edit_Val(function_name, FunctionCtx(ctx, temp_default_scope));
            return 0;
        }
        auto default_val_list = targs_list->test();
        auto val_riter = default_val_list.rbegin();
        auto default_name_list = targs_list->tfpdef();
        auto name_riter = default_name_list.rbegin();
        for (; val_riter != default_val_list.rend(); val_riter++, name_riter++) {
            ValName val_name = (*name_riter)->NAME()->getText();
            temp_default_scope[val_name] = visitTest(*val_riter);
        }
        Edit_Val(function_name, FunctionCtx(ctx, temp_default_scope));
        // return visitChildren(ctx);
        return 0;
    }
    antlrcpp::Any visitFunc(FunctionCtx function_ctx)
    {
        Scope temp_scope = function_ctx.default_scope;
        auto ctx = function_ctx.Get_Ctx();
        scope_stack.push(temp_scope);
        auto ans = NULL;
        auto stmt_list = ctx->suite()->stmt();
        antlrcpp::Any return_val = NULL;
        for (auto stmt_iter = stmt_list.begin(); stmt_iter != stmt_list; stmt_iter++) {
            auto ans = visitStmt(*stmt_iter);
            if (ans.is<ReturnVal>()) {
                return_val = ans.as<ReturnVal>().Get_Val();
                break;
            }
        }
        Get_Val(return_val);
        scope_stack.pop();
        return return_val;
    }
    antlrcpp::Any visitFunc(FunctionCtx function_ctx, std::vector<Python3Parser::ArgumentContext*> argu_list)
    {
        Scope temp_scope = function_ctx.default_scope;
        auto ctx = function_ctx.Get_Ctx();
        auto targs_list = ctx->parameters()->typedargslist();
        if (targs_list) {
            auto name_list = targs_list->tfpdef();
            auto name_iter = name_list.begin();
            for (auto argu_iter = argu_list.begin(); argu_iter != argu_list.end(); argu_iter++) {
                auto test_list = (*argu_iter)->test();
                if (test_list.size() == 1) {
                    auto val_name = (*name_iter)->NAME()->getText();
                    auto val = visitArgument(*argu_iter);
                    Get_Val(val);
                    temp_scope[val_name] = val;
                    name_iter++;
                }
                if (test_list.size() == 2) {
                    auto val_name = visitTest(*test_list.begin()).as<ValName>();
                    auto val = visitArgument(*argu_iter);
                    Get_Val(val);
                    temp_scope[val_name] = val;
                }
            }
        }
        scope_stack.push(temp_scope);
        // auto ans = NULL;
        auto stmt_list = ctx->suite()->stmt();
        antlrcpp::Any return_val = NULL;
        if (!stmt_list.empty())
            for (auto stmt_iter = stmt_list.begin(); stmt_iter != stmt_list.end(); stmt_iter++) {
                auto ans = visitStmt(*stmt_iter);
                if (ans.is<ReturnVal>()) {
                    return_val = ans.as<ReturnVal>().Get_Val();
                    break;
                }
            }
        else {
            auto simple_stmt = ctx->suite()->simple_stmt();
            auto ans = visitSimple_stmt(simple_stmt);
            if (ans.is<ReturnVal>()) {
                return_val = ans.as<ReturnVal>().Get_Val();
            }
        }
        Get_Val(return_val);
        scope_stack.pop();
        return return_val;
    }

    virtual antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext* ctx) override
    {
        //if (ctx->testlist())
            //std::cerr << "Returning :" << ctx->testlist()->getText() << std::endl;
        antlrcpp::Any ans = NULL;
        if (ctx->testlist())
            ans = visitTestlist(ctx->testlist());
        // return visitChildren(ctx);
        return ReturnVal(ans);
    }
};

#endif //PYTHON_INTERPRETER_EVALVISITOR_H

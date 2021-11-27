// Generated from D:/VSC/Python-Assignment-2021-master/generated\Python3.g4 by ANTLR 4.9.2
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link Python3Parser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface Python3Visitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link Python3Parser#file_input}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFile_input(Python3Parser.File_inputContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#funcdef}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFuncdef(Python3Parser.FuncdefContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#parameters}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitParameters(Python3Parser.ParametersContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#typedargslist}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypedargslist(Python3Parser.TypedargslistContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#tfpdef}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTfpdef(Python3Parser.TfpdefContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStmt(Python3Parser.StmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#simple_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSimple_stmt(Python3Parser.Simple_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#small_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSmall_stmt(Python3Parser.Small_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#expr_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpr_stmt(Python3Parser.Expr_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#augassign}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAugassign(Python3Parser.AugassignContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#flow_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFlow_stmt(Python3Parser.Flow_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#break_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitBreak_stmt(Python3Parser.Break_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#continue_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitContinue_stmt(Python3Parser.Continue_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#return_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitReturn_stmt(Python3Parser.Return_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#compound_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitCompound_stmt(Python3Parser.Compound_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#if_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitIf_stmt(Python3Parser.If_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#while_stmt}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitWhile_stmt(Python3Parser.While_stmtContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#suite}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSuite(Python3Parser.SuiteContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#test}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTest(Python3Parser.TestContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#or_test}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitOr_test(Python3Parser.Or_testContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#and_test}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAnd_test(Python3Parser.And_testContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#not_test}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNot_test(Python3Parser.Not_testContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#comparison}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitComparison(Python3Parser.ComparisonContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#comp_op}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitComp_op(Python3Parser.Comp_opContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#arith_expr}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArith_expr(Python3Parser.Arith_exprContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#addorsub_op}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAddorsub_op(Python3Parser.Addorsub_opContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTerm(Python3Parser.TermContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#muldivmod_op}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMuldivmod_op(Python3Parser.Muldivmod_opContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#factor}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFactor(Python3Parser.FactorContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#atom_expr}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAtom_expr(Python3Parser.Atom_exprContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#trailer}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTrailer(Python3Parser.TrailerContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#atom}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAtom(Python3Parser.AtomContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#testlist}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTestlist(Python3Parser.TestlistContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#arglist}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArglist(Python3Parser.ArglistContext ctx);
	/**
	 * Visit a parse tree produced by {@link Python3Parser#argument}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArgument(Python3Parser.ArgumentContext ctx);
}
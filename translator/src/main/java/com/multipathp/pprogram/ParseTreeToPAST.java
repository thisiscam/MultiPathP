package com.multipathp.pprogram;

import com.multipathp.pparser.pBaseVisitor;
import com.multipathp.pparser.pParser;
import com.multipathp.pprogram.ast.*;
import com.multipathp.pprogram.types.PType;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.RuleNode;
import sun.tools.tree.IdentifierExpression;

import java.util.*;

public class ParseTreeToPAST extends ParseTreeSetParser.ASTSetVisitorBase<Void> {
    private PTypeConverter typeConverter;
    private PProgram.Builder program;
    private PEvent.Builder event;
    private PMachine.Builder machine;
    private PFunction.Builder function;

    private int randomSiteCount = 0;

    public ParseTreeToPAST(ParseTreeSetParser.ParseTreeSet set) {
        super(set);
        program = new PProgram.Builder().setASTSet(set);
        typeConverter = new PTypeConverter(program);
    }

    public PProgram getProgram() {
        visit();
        return program.build();
    }

    @Override
    public Void visitProgram(pParser.ProgramContext ctx) {
        return super.visitProgram(ctx);
    }

    @Override
    public Void visitAnnotation(pParser.AnnotationContext ctx) {
        ErrorReporter.error("annotation not supported", ctx, set);
        return null;
    }

    @Override
    public Void visitType_def_decl(pParser.Type_def_declContext ctx) {
        program.putTypedefs(ctx.ID().getText(), ctx.ptype().accept(typeConverter));
        return visitChildren(ctx);
    }

    @Override
    public Void visitEvent_decl(pParser.Event_declContext ctx) {
        event = new PEvent.Builder().setName(ctx.ID().getText());
        ctx.ev_type_or_none().accept(this);
        program.addEventDecls(event.build());
        return null;
    }

    @Override
    public Void visitEv_type_or_none(pParser.Ev_type_or_noneContext ctx) {
        if (ctx.ptype() != null) {
            event.setPayloadType(ctx.ptype().accept(typeConverter));
        }
        return null;
    }

    @Override
    public Void visitMachine_decl(pParser.Machine_declContext ctx) {
        machine = new PMachine.Builder();
        visitChildren(ctx);
        program.addMachines(machine.build());
        return null;
    }

    @Override
    public Void visitMachine_name_decl_regular(pParser.Machine_name_decl_regularContext ctx) {
        machine.setName(ctx.ID().getText());
        return super.visitMachine_name_decl_regular(ctx);
    }

    @Override
    public Void visitIs_main(pParser.Is_mainContext ctx) {
        machine.setMain(ctx.MAIN() != null);
        return null;
    }

    @Override
    public Void visitMachine_name_decl_model(pParser.Machine_name_decl_modelContext ctx) {
        ErrorReporter.warn("keyword 'model' ignored", ctx, set);
        machine.setName(ctx.ID().getText()).setMain(false);
        return super.visitMachine_name_decl_model(ctx);
    }

    @Override
    public Void visitMachine_name_decl_spec(pParser.Machine_name_decl_specContext ctx) {
        machine.setName(ctx.ID().getText());
        machine.setSpec(true).setMain(false);
        ctx.observes_list().accept(this);
        return super.visitMachine_name_decl_spec(ctx);
    }

    @Override
    public Void visitObserves_list(pParser.Observes_listContext ctx) {
        Collection<PEvent> events = ctx.event_list().accept(new UtilVisitors.EventListVisitor());
        Map<String, PEvent> eventsMap = new HashMap<>();
        for(PEvent e : events) {
            eventsMap.put(e.getName(), e);
        }
        machine.putAllObservedEvents(eventsMap);
        return null;
    }

    @Override
    public Void visitVar_decl(pParser.Var_declContext ctx) {
        final PType type = ctx.ptype().accept(typeConverter);
        ctx.var_list().accept(
                new pBaseVisitor<Void>() {
                    @Override
                    public Void visitVar_list(pParser.Var_listContext ctx) {
                        machine.putVarDecls(ctx.ID().getText(), type);
                        return null;
                    }
                }
        );
        if(ctx.annotation_set() != null) {
            ctx.annotation_set().accept(this);
        }
        return null;
    }

    @Override
    public Void visitFun_decl(pParser.Fun_declContext ctx) {
        function = new PFunction.Builder();
        visitChildren(ctx);
        machine.putFunDecls(function.build());
        return null;
    }

    @Override
    public Void visitFun_name_decl(pParser.Fun_name_declContext ctx) {
        function.setName(ctx.ID().getText());
        return null;
    }

    @Override
    public Void visitParams_or_none(pParser.Params_or_noneContext ctx) {
        if (ctx.nmd_tup_type_list() != null) {
            function.setParams(ctx.nmd_tup_type_list().accept(new UtilVisitors.NamedTupleTypeListVisitor(typeConverter)));
        }
        return null;
    }

    @Override
    public Void visitIs_model(pParser.Is_modelContext ctx) {
        function.setModel(ctx.MODEL() != null);
        return null;
    }

    @Override
    public Void visitRet_type_or_none(pParser.Ret_type_or_noneContext ctx) {
        function.setRetType(ctx.ptype() != null ? ctx.ptype().accept(typeConverter) : PType.VOID);
        return null;
    }

    @Override
    public Void visitStmt_block(pParser.Stmt_blockContext ctx) {
        StmtBlock.Builder builder = new StmtBlock.Builder();
        ctx.local_var_decl_list().accept(new ParseTreeSetParser.ASTSetVisitorBase<Void>(set){
            PType type;
            @Override
            public Void visitLocal_var_decl(pParser.Local_var_declContext ctx) {
                type = ctx.ptype().accept(typeConverter);
                return super.visitLocal_var_decl(ctx);
            }

            @Override
            public Void visitLocal_var_list(pParser.Local_var_listContext ctx) {
                builder.addLocalVarDecls(ctx.ID().getText(), type);
                return super.visitLocal_var_list(ctx);
            }
        });
        StmtListStmt stmtList;
        if(ctx.stmt_list() == null) {
            stmtList = new StmtListStmt.Builder().build();
        } else {
            stmtList = (StmtListStmt) ctx.stmt_list().accept(new StmtVisitor()).orElse(new StmtListStmt.Builder().build());
        }
        builder.setStmtList(stmtList);
        function.setStmtBlock(builder.build());
        return null;
    }

    @Override
    public Void visitPayload_var_decl_or_none(pParser.Payload_var_decl_or_noneContext ctx) {
        if (ctx.ID() != null) {
           function.addParams(ctx.ID().getText(), ctx.ptype().accept(typeConverter));
        }
        return null;
    }

    @Override
    public Void visitPayload_var_decl_or_none_ref(pParser.Payload_var_decl_or_none_refContext ctx) {
        if (ctx.ID() != null) {
            function.addParams(ctx.ID().getText(), ctx.ptype().accept(typeConverter));
        }
        return null;
    }

    @Override
    public Void visitGroup(pParser.GroupContext ctx) {
        ErrorReporter.error("group is not supported", ctx, set);
        return null;
    }

    PMachineState.Builder state;
    @Override
    public Void visitState_decl(pParser.State_declContext ctx) {
        state = new PMachineState.Builder()
                .setName(ctx.ID().getText())
                .setStart(!ctx.is_start_state_or_none().isEmpty());
        visitChildren(ctx);
        machine.addStateDecls(state.build());
        return null;
    }

    @Override
    public Void visitState_body_item_entry_unnamed(pParser.State_body_item_entry_unnamedContext ctx) {
        function = new PFunction.Builder().setName(state.getName() + "EntryImpl").setRetType(PType.VOID);
        visitChildren(ctx);
        state.setEntryFunctionName(function.getName());
        machine.putFunDecls(function.build());
        return null;
    }

    @Override
    public Void visitState_body_item_entry_fn_named(pParser.State_body_item_entry_fn_namedContext ctx) {
        state.setEntryFunctionName(ctx.ID().getText());
        return null;
    }

    @Override
    public Void visitState_body_item_exit_unnamed(pParser.State_body_item_exit_unnamedContext ctx) {
        function = new PFunction.Builder().setName(state.getName() + "ExitImpl").setRetType(PType.VOID);
        visitChildren(ctx);
        state.setExitFunctionName(function.getName());
        machine.putFunDecls(function.build());
        return null;
    }

    @Override
    public Void visitState_body_item_exit_fn_named(pParser.State_body_item_exit_fn_namedContext ctx) {
        state.setExitFunctionName(ctx.ID().getText());
        return null;
    }

    @Override
    public Void visitState_body_item_defer(pParser.State_body_item_deferContext ctx) {
        state.putAllDeferedEvents(ctx.non_default_event_list().accept(new UtilVisitors.NonDefaultEventListVisitor()));
        return null;
    }

    @Override
    public Void visitState_body_item_ignore(pParser.State_body_item_ignoreContext ctx) {
        Collection<PEvent> ignoreEvents = ctx.non_default_event_list().accept(new UtilVisitors.NonDefaultEventListVisitor()).values();
        state.addTransition((String) null, ignoreEvents, null, false);
        return null;
    }


    private Collection<PEvent> eventList;
    @Override
    public Void visitOn_event_list(pParser.On_event_listContext ctx) {
        eventList = ctx.event_list().accept(new UtilVisitors.EventListVisitor());
        return null;
    }

    private void addTransitionsHelper(RuleNode ctx, String transitionFunctionName, String toStateName, boolean isPush) {
        visitChildren(ctx);
        state.addTransition(transitionFunctionName, eventList, toStateName, isPush);
    }

    private void addTransitionsHelper(RuleNode ctx, String toStateName, boolean isPush) {
        function.setRetType(PType.VOID);
        visitChildren(ctx);
        state.addTransition(function, eventList, toStateName, isPush);
        machine.putFunDecls(function.build());
    }

    private int anonymousHandlerCounter = 0;

    @Override
    public Void visitState_body_item_on_e_do_fn_named(pParser.State_body_item_on_e_do_fn_namedContext ctx) {
        addTransitionsHelper(ctx, ctx.ID().getText(), null, false);
        return null;
    }

    @Override
    public Void visitState_body_item_on_e_do_unnamed(pParser.State_body_item_on_e_do_unnamedContext ctx) {
        function = new PFunction.Builder().setName(String.format("HandlerImpl%d", anonymousHandlerCounter++));
        addTransitionsHelper(ctx, null, false);
        return null;
    }

    @Override
    public Void visitState_body_item_push(pParser.State_body_item_pushContext ctx) {
        addTransitionsHelper(ctx, ctx.state_target().getText(), true);
        return null;
    }

    @Override
    public Void visitState_body_item_on_e_goto(pParser.State_body_item_on_e_gotoContext ctx) {
        visitChildren(ctx);
        state.addTransition((String)null, eventList, ctx.state_target().getText(), false);
        return null;
    }

    @Override
    public Void visitState_body_item_on_e_goto_with_unnamed(pParser.State_body_item_on_e_goto_with_unnamedContext ctx) {
        function = new PFunction.Builder().setName(String.format("AnonymousHandler_%d", anonymousHandlerCounter++));
        addTransitionsHelper(ctx, ctx.state_target().getText(), false);
        return null;
    }

    @Override
    public Void visitState_body_item_on_e_goto_with_fn_named(pParser.State_body_item_on_e_goto_with_fn_namedContext ctx) {
        addTransitionsHelper(ctx, ctx.ID().getText(), ctx.state_target().getText(), false);
        return null;
    }

    @Override
    public Void visitState_target(pParser.State_targetContext ctx) {
        if(ctx.state_target() != null) {
            ErrorReporter.error("state target . not supported", ctx, set);
        }
        return null;
    }

    class StmtVisitor extends ParseTreeSetParser.ASTSetVisitorBase<Optional<Stmt>> {
        StmtVisitor() {
            super(ParseTreeToPAST.this.set);
        }

        private Exp exp(ParserRuleContext ctx) {
            return ctx.accept(new ExpVisitor());
        }

        private Stmt stmt(ParserRuleContext ctx) {
            return ctx.accept(this).orElse(new StmtListStmt.Builder().build());
        }

        @Override
        public Optional<Stmt> visitStmt_list(pParser.Stmt_listContext ctx) {
            StmtListStmt.Builder builder = new StmtListStmt.Builder();
            ctx.stmt().accept(this).ifPresent((builder::addStmts));
            if(ctx.stmt_list() != null) {
                ctx.stmt_list().accept(this).ifPresent(stmt -> {
                    builder.addAllStmts(((StmtListStmt)stmt).getStmts());
                });
            }
            if(builder.getStmts().size() > 0) {
                return Optional.of(builder.build());
            } else {
                return Optional.empty();
            }
        }

        @Override
        public Optional<Stmt> visitStmt_semicolon(pParser.Stmt_semicolonContext ctx) {
            return Optional.empty();
        }

        @Override
        public Optional<Stmt> visitStmt_lrbrace(pParser.Stmt_lrbraceContext ctx) {
            return Optional.empty();
        }

        @Override
        public Optional<Stmt> visitStmt_pop(pParser.Stmt_popContext ctx) {
            return Optional.of(new PopStmt.Builder().build());
        }

        @Override
        public Optional<Stmt> visitStmt_stmt_list(pParser.Stmt_stmt_listContext ctx) {
            return ctx.stmt_list().accept(this);
        }

        @Override
        public Optional<Stmt> visitStmt_assert(pParser.Stmt_assertContext ctx) {
            return Optional.of(
                    new AssertStmt.Builder()
                            .setExp(exp(ctx.exp()))
                            .setMessage("")
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_assert_str(pParser.Stmt_assert_strContext ctx) {
            return Optional.of(
                    new AssertStmt.Builder()
                            .setExp(exp(ctx.exp()))
                            .setMessage(ctx.STR().getText())
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_print(pParser.Stmt_printContext ctx) {
            return Optional.of(
                    new PrintStmt.Builder().setString(ctx.STR().getText()).build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_return(pParser.Stmt_returnContext ctx) {
            return Optional.of(new ReturnStmt.Builder().build());
        }

        @Override
        public Optional<Stmt> visitStmt_return_exp(pParser.Stmt_return_expContext ctx) {
            return Optional.of(new ReturnStmt.Builder().setExpression(exp(ctx.exp())).build());
        }


        @Override
        public Optional<Stmt> visitStmt_assign(pParser.Stmt_assignContext ctx) {
            Exp targetExp = exp(ctx.exp(0));
            Exp rhs = exp(ctx.exp(1));
            return Optional.of(
                    new AssignStmt.Builder()
                            .setTarget(targetExp)
                            .setExpression(rhs)
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_remove(pParser.Stmt_removeContext ctx) {
            return Optional.of(
                    new RemoveStmt.Builder()
                            .setTarget(exp(ctx.exp(0)))
                            .setExpression(exp(ctx.exp(1)))
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_insert(pParser.Stmt_insertContext ctx) {
            return Optional.of(
                    new InsertStmt.Builder()
                            .setTarget(exp(ctx.exp(0)))
                            .setExpression(exp(ctx.exp(1)))
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_while(pParser.Stmt_whileContext ctx) {
            return Optional.of(
                    new WhileStmt.Builder()
                            .setCondition(exp(ctx.exp()))
                            .setStmt(stmt(ctx.stmt()))
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_if_then(pParser.Stmt_if_thenContext ctx) {
            return Optional.of(
                new ConditionalStmt.Builder()
                    .setCondition(exp(ctx.exp()))
                    .setThenBranch(stmt(ctx.stmt()))
                    .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_if_then_else(pParser.Stmt_if_then_elseContext ctx) {
            return Optional.of(
                    new ConditionalStmt.Builder()
                            .setCondition(exp(ctx.exp()))
                            .setThenBranch(stmt(ctx.stmt(0)))
                            .setElseBranch(ctx.stmt(1).accept(this).orElse(null))
                            .build()
            );
        }

        private Exp singleExpOrTuple(pParser.Single_expr_arg_listContext ctx) {
            List<Exp> arguments = ctx.accept(
                    new UtilVisitors.ArgumentListVisitor<>(set, new ExpVisitor())
            );
            if(arguments.size() == 0) {
                return null;
            } else if(arguments.size() == 1) {
                return arguments.get(0);
            } else {
                return new TupleExp.Builder().addAllArguments(arguments).build();
            }
        }

        @Override
        public Optional<Stmt> visitStmt_new(pParser.Stmt_newContext ctx) {
            return Optional.of(
                    new ExpStmt.Builder()
                            .setExpression(
                                new NewExp.Builder()
                                    .setMachineIdentifier(ctx.ID().getText())
                                    .build()
                            ).build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_new_with_arguments(pParser.Stmt_new_with_argumentsContext ctx) {
            return Optional.of(
                    new ExpStmt.Builder()
                            .setExpression(
                                    new NewExp.Builder()
                                            .setMachineIdentifier(ctx.ID().getText())
                                            .setPayloadExpression(singleExpOrTuple(ctx.single_expr_arg_list()))
                                            .build()
                            ).build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_call(pParser.Stmt_callContext ctx) {
            return Optional.of(
                    new ExpStmt.Builder()
                            .setExpression(
                                    new CallExp.Builder()
                                            .setFunctionName(ctx.ID().getText())
                                            .build()
                            ).build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_call_with_arguments(pParser.Stmt_call_with_argumentsContext ctx) {
            if(ctx.ID().getText().equals("BroadCastAcceptors")) {
                int x = 1;
            }
            return Optional.of(
                    new ExpStmt.Builder()
                            .setExpression(
                                    new CallExp.Builder()
                                            .setFunctionName(ctx.ID().getText())
                                            .addAllArguments(ctx.expr_arg_list().accept(
                                                    new UtilVisitors.ArgumentListVisitor<>(set, new ExpVisitor())
                                            ))
                                            .build()
                            ).build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_raise(pParser.Stmt_raiseContext ctx) {
            return Optional.of(
                    new RaiseStmt.Builder()
                        .setEventExpression(exp(ctx.exp()))
                        .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_raise_with_arguments(pParser.Stmt_raise_with_argumentsContext ctx) {
            return Optional.of(
                        new RaiseStmt.Builder()
                            .setEventExpression(exp(ctx.exp()))
                            .setPayloadExpression(singleExpOrTuple(ctx.single_expr_arg_list()))
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_send(pParser.Stmt_sendContext ctx) {
            ctx.qualifier_or_none().accept(this);
            return Optional.of(
                    new SendStmt.Builder()
                            .setTargetExpression(exp(ctx.exp(0)))
                            .setEventExpression(exp(ctx.exp(1)))
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_send_with_arguments(pParser.Stmt_send_with_argumentsContext ctx) {
            ctx.qualifier_or_none().accept(this);
            return Optional.of(
                    new SendStmt.Builder()
                            .setTargetExpression(exp(ctx.exp(0)))
                            .setEventExpression(exp(ctx.exp(1)))
                            .setPayloadExpression(singleExpOrTuple(ctx.single_expr_arg_list()))
                            .build()
            );
        }

        @Override
        public Optional<Stmt> visitQualifier_or_none(pParser.Qualifier_or_noneContext ctx) {
            if(ctx.getChildCount() > 0) {
                System.out.print(ctx.getText());
                ErrorReporter.error("qualifier not supported", ctx, set);
            }
            return null;
        }

        @Override
        public Optional<Stmt> visitStmt_announce(pParser.Stmt_announceContext ctx) {
            return Optional.of(
              new AnnounceStmt.Builder()
                      .setEventExpression(exp(ctx.exp()))
                      .build()
            );
        }

        @Override
        public Optional<Stmt> visitStmt_announce_with_arguments(pParser.Stmt_announce_with_argumentsContext ctx) {
            return Optional.of(
              new AnnounceStmt.Builder()
                    .setEventExpression(exp(ctx.exp()))
                    .setPayloadExpression(singleExpOrTuple(ctx.single_expr_arg_list()))
                    .build()
            );
        }

        class ExpVisitor extends ParseTreeSetParser.ASTSetVisitorBase<Exp> {
            ExpVisitor() {
                super(StmtVisitor.this.set);
            }

            @Override
            public Exp visitExp_true(pParser.Exp_trueContext ctx) {
                return new BoolLiteralExp.Builder().setKind(BoolLiteralExp.Kind.TRUE).build();
            }

            @Override
            public Exp visitExp_false(pParser.Exp_falseContext ctx) {
                return new BoolLiteralExp.Builder().setKind(BoolLiteralExp.Kind.FALSE).build();
            }

            @Override
            public Exp visitExp_this(pParser.Exp_thisContext ctx) {
                return new ThisExp.Builder().build();
            }

            @Override
            public Exp visitExp_nondet(pParser.Exp_nondetContext ctx) {
                return new NondetExp.Builder().setFair(false).setSite(randomSiteCount++).build();
            }

            @Override
            public Exp visitExp_fairnondet(pParser.Exp_fairnondetContext ctx) {
                return new NondetExp.Builder().setFair(true).setSite(randomSiteCount++).build();
            }

            @Override
            public Exp visitExp_null(pParser.Exp_nullContext ctx) {
                return new NullExp.Builder().build();
            }

            @Override
            public Exp visitExp_halt(pParser.Exp_haltContext ctx) {
                return new HaltExp.Builder().build();
            }

            @Override
            public Exp visitExp_int(pParser.Exp_intContext ctx) {
                return new IntExp.Builder().setValue(Integer.parseInt(ctx.INT().getText())).build();
            }

            @Override
            public Exp visitExp_id(pParser.Exp_idContext ctx) {
                return new IdExp.Builder().setName(ctx.ID().getText()).build();
            }

            @Override
            public Exp visitExp_getattr(pParser.Exp_getattrContext ctx) {
                return new GetAttributeExp.Builder()
                        .setTargetExpression(ctx.exp_0().accept(this))
                        .setAttributeName(ctx.ID().getText())
                        .build();
            }

            @Override
            public Exp visitExp_getidx(pParser.Exp_getidxContext ctx) {
                return new GetIndexExp.Builder()
                        .setTargetExpression(ctx.exp_0().accept(this))
                        .setIndex(Integer.parseInt(ctx.INT().getText()))
                        .build();
            }

            @Override
            public Exp visitExp_getitem(pParser.Exp_getitemContext ctx) {
                return new GetItemExp.Builder()
                        .setTargetExpression(ctx.exp_0().accept(this))
                        .setIndexerExpression(ctx.exp().accept(this))
                        .build();
            }

            @Override
            public Exp visitExp_grouped(pParser.Exp_groupedContext ctx) {
                return new GroupExp.Builder().setExpression(ctx.exp().accept(this)).build();
            }

            @Override
            public Exp visitExp_keys(pParser.Exp_keysContext ctx) {
                return new KeysExp.Builder().setExpression(ctx.exp().accept(this)).build();
            }

            @Override
            public Exp visitExp_values(pParser.Exp_valuesContext ctx) {
                return new ValuesExp.Builder().setExpression(ctx.exp().accept(this)).build();
            }

            @Override
            public Exp visitExp_sizeof(pParser.Exp_sizeofContext ctx) {
                return new SizeofExp.Builder().setExpression(ctx.exp().accept(this)).build();
            }

            @Override
            public Exp visitExp_default(pParser.Exp_defaultContext ctx) {
                return new DefaultExp.Builder().setTypeForDefault(ctx.ptype().accept(typeConverter)).build();
            }

            @Override
            public Exp visitExp_new(pParser.Exp_newContext ctx) {
                return new NewExp.Builder()
                                .setMachineIdentifier(ctx.ID().getText())
                                .build();
            }

            @Override
            public Exp visitExp_new_with_arguments(pParser.Exp_new_with_argumentsContext ctx) {
                return new NewExp.Builder()
                                .setMachineIdentifier(ctx.ID().getText())
                                .setPayloadExpression(singleExpOrTuple(ctx.single_expr_arg_list()))
                                .build();
            }

            @Override
            public Exp visitExp_tuple_1_elem(pParser.Exp_tuple_1_elemContext ctx) {
                return new TupleExp.Builder().addArguments(ctx.exp().accept(this)).build();
            }

            @Override
            public Exp visitExp_tuple_n_elems(pParser.Exp_tuple_n_elemsContext ctx) {
                Exp argumentHead = ctx.exp().accept(this);
                Collection<Exp> argumentRest = ctx.expr_arg_list().accept(new UtilVisitors.ArgumentListVisitor<>(set, this));
                TupleExp x = new TupleExp.Builder().addArguments(argumentHead).addAllArguments(argumentRest).build();
                return x;
            }

            @Override
            public Exp visitExp_call(pParser.Exp_callContext ctx) {
                return new CallExp.Builder()
                                .setFunctionName(ctx.ID().getText())
                                .build();
            }

            @Override
            public Exp visitExp_call_with_arguments(pParser.Exp_call_with_argumentsContext ctx) {
                return new CallExp.Builder()
                                .setFunctionName(ctx.ID().getText())
                                .addAllArguments(ctx.expr_arg_list().accept(
                                        new UtilVisitors.ArgumentListVisitor<>(set, this)
                                ))
                                .build();
            }

            @Override
            public Exp visitExp_named_tuple_1_elem(pParser.Exp_named_tuple_1_elemContext ctx) {
                return new NamedTupleExp.Builder().addNamedPair(ctx.ID().getText(), ctx.exp().accept(this)).build();
            }

            @Override
            public Exp visitExp_named_tuple_n_elems(pParser.Exp_named_tuple_n_elemsContext ctx) {
                NamedTupleExp.Builder builder = new NamedTupleExp.Builder();
                builder.addNamedPair(ctx.ID().getText(), ctx.exp().accept(this));
                ctx.nmd_expr_arg_list().accept(new pBaseVisitor<Void>(){
                    @Override
                    public Void visitNmd_expr_arg_list(pParser.Nmd_expr_arg_listContext ctx) {
                        builder.addNamedPair(ctx.ID().getText(), ctx.exp().accept(ExpVisitor.this));
                        if(ctx.nmd_expr_arg_list() != null) {
                            ctx.nmd_expr_arg_list().accept(this);
                        }
                        return null;
                    }
                });
                return builder.build();
            }

            @Override
            public Exp visitExp_1(pParser.Exp_1Context ctx) {
                if(ctx.MINUS() != null) {
                    return new MinusExp.Builder().setExpression(ctx.exp_0().accept(this)).build();
                } else if(ctx.LNOT() != null) {
                    return new NotExp.Builder().setExpression(ctx.exp_0().accept(this)).build();
                } else {
                    return ctx.exp_0().accept(this);
                }
            }

            @Override
            public Exp visitExp_2(pParser.Exp_2Context ctx) {
                ArithmeticExp.Builder builder = new ArithmeticExp.Builder();
                if(ctx.MUL() != null) {
                    builder.setKind(ArithmeticExp.Kind.MUL);
                } else if(ctx.DIV() != null) {
                    builder.setKind(ArithmeticExp.Kind.DIV);
                } else {
                    return ctx.exp_1().accept(this);
                }
                builder.setLeft(ctx.exp_2().accept(this)).setRight(ctx.exp_1().accept(this));
                return builder.build();
            }

            @Override
            public Exp visitExp_3(pParser.Exp_3Context ctx) {
                ArithmeticExp.Builder builder = new ArithmeticExp.Builder();
                if(ctx.PLUS() != null) {
                    builder.setKind(ArithmeticExp.Kind.ADD);
                } else if(ctx.MINUS() != null) {
                    builder.setKind(ArithmeticExp.Kind.SUB);
                } else {
                    return ctx.exp_2().accept(this);
                }
                builder.setLeft(ctx.exp_3().accept(this)).setRight(ctx.exp_2().accept(this));
                return builder.build();
            }

            @Override
            public Exp visitExp_4(pParser.Exp_4Context ctx) {
                if(ctx.AS() != null) {
                    return new CastExp.Builder()
                            .setExpression(ctx.exp_4().accept(this))
                            .setCastToType(ctx.ptype().accept(typeConverter))
                            .build();
                } else {
                    return ctx.exp_3().accept(this);
                }
            }

            @Override
            public Exp visitExp_5(pParser.Exp_5Context ctx) {
                if(ctx.IN() != null) {
                    return new InExp.Builder()
                            .setLeft(ctx.exp_4(0).accept(this))
                            .setRight(ctx.exp_4(1).accept(this))
                            .build();
                }
                ComparisonExp.Builder builder = new ComparisonExp.Builder();
                if(ctx.LT() != null) {
                    builder.setKind(ComparisonExp.Kind.LT);
                } else if(ctx.LE() != null) {
                    builder.setKind(ComparisonExp.Kind.LE);
                } else if(ctx.GT() != null) {
                    builder.setKind(ComparisonExp.Kind.GT);
                } else if(ctx.GE() != null) {
                    builder.setKind(ComparisonExp.Kind.GE);
                } else {
                    return ctx.exp_4(0).accept(this);
                }
                return builder
                        .setLeft(ctx.exp_4(0).accept(this))
                        .setRight(ctx.exp_4(1).accept(this))
                        .build();
            }

            @Override
            public Exp visitExp_6(pParser.Exp_6Context ctx) {
                ComparisonExp.Builder builder = new ComparisonExp.Builder();
                if(ctx.NE() != null) {
                    builder.setKind(ComparisonExp.Kind.NE);
                } else if (ctx.EQ() != null) {
                    builder.setKind(ComparisonExp.Kind.EQ);
                } else {
                    return ctx.exp_5(0).accept(this);
                }
                return builder
                        .setLeft(ctx.exp_5(0).accept(this))
                        .setRight(ctx.exp_5(1).accept(this))
                        .build();
            }

            @Override
            public Exp visitExp_7(pParser.Exp_7Context ctx) {
                if(ctx.LAND() != null) {
                    return new BinaryLogicExp.Builder()
                            .setKind(BinaryLogicExp.Kind.AND)
                            .setLeft(ctx.exp_7().accept(this))
                            .setRight(ctx.exp_6().accept(this))
                            .build();
                } else {
                    return ctx.exp_6().accept(this);
                }
            }

            @Override
            public Exp visitExp(pParser.ExpContext ctx) {
                if(ctx.LOR() != null) {
                    return new BinaryLogicExp.Builder()
                            .setKind(BinaryLogicExp.Kind.OR)
                            .setLeft(ctx.exp().accept(this))
                            .setRight(ctx.exp_7().accept(this))
                            .build();
                } else {
                    return ctx.exp_7().accept(this);
                }
            }
        }
    }
}
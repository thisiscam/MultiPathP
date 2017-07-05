package com.multipathp.pprogram;

import com.multipathp.pparser.pBaseVisitor;
import com.multipathp.pparser.pParser;
import com.multipathp.pprogram.ast.Exp;
import com.multipathp.pprogram.types.PType;

import java.util.*;

class UtilVisitors {
    static class TupleTypeListVisitor extends pBaseVisitor<Collection<PType>> {
        private final PTypeConverter converter;
        ArrayList<PType> ret = new ArrayList<>();

        public TupleTypeListVisitor(PTypeConverter converter) {
            this.converter = converter;
        }

        @Override
        public Collection<PType> visitTup_type_list(pParser.Tup_type_listContext ctx) {
            ret.add(ctx.ptype().accept(converter));
            if (ctx.tup_type_list() != null) {
                ctx.tup_type_list().accept(this);
            }
            return ret;
        }
    }

    static class NamedTupleTypeListVisitor extends pBaseVisitor<LinkedHashMap<String, PType>> {
        private final PTypeConverter converter;
        LinkedHashMap<String, PType> ret = new LinkedHashMap<>();

        public NamedTupleTypeListVisitor(PTypeConverter converter) {
            this.converter = converter;
        }

        @Override
        public LinkedHashMap<String, PType> visitNmd_tup_type_list(pParser.Nmd_tup_type_listContext ctx) {
            ret.put(ctx.ID().getText(), ctx.ptype().accept(converter));
            if (ctx.nmd_tup_type_list() != null) {
                ctx.nmd_tup_type_list().accept(this);
            }
            return ret;
        }
    }

    static class EventListVisitor extends pBaseVisitor<Collection<PEvent>> {
        ArrayList<PEvent> ret = new ArrayList<>();

        @Override
        public Collection<PEvent> visitEvent_list(pParser.Event_listContext ctx) {
            ret.add(new PEvent.Builder().setName(ctx.event_id().getText()).build());
            if (ctx.event_list() != null) {
                ctx.event_list().accept(this);
            }
            return ret;
        }
    }

    static class NonDefaultEventListVisitor extends pBaseVisitor<Map<String, PEvent>>
    {
        Map<String, PEvent> ret = new HashMap<>();

        @Override
        public Map<String, PEvent> visitNon_default_event_list(pParser.Non_default_event_listContext ctx) {
            String name = ctx.non_default_event_id().getText();
            ret.put(name, new PEvent.Builder().setName(name).build());
            if (ctx.non_default_event_list() != null) {
                ctx.non_default_event_list().accept(this);
            }
            return ret;
        }
    }

    static class ArgumentListVisitor<T extends ParseTreeSetParser.ASTSetVisitorBase<Exp>> extends ParseTreeSetParser.ASTSetVisitorBase<ArrayList<Exp>>
    {
        ArrayList<Exp> arguments = new ArrayList<>();
        T expVisitor;

        public ArgumentListVisitor(ParseTreeSetParser.ParseTreeSet set, T expVisitor) {
            super(set);
            this.expVisitor = expVisitor;
        }

        @Override
        public ArrayList<Exp> visitQualifier_or_none(pParser.Qualifier_or_noneContext ctx) {
            if(ctx.getChildCount() > 0) {
                ErrorReporter.error("qualifier not supported", ctx, set);
            }
            return null;
        }

        @Override
        public ArrayList<Exp> visitExpr_arg_list(pParser.Expr_arg_listContext ctx) {
            arguments.add(ctx.exp().accept(expVisitor));
            if(ctx.expr_arg_list() != null) {
                ctx.expr_arg_list().accept(this);
            }
            return arguments;
        }

        @Override
        public ArrayList<Exp> visitSingle_expr_arg_list(pParser.Single_expr_arg_listContext ctx) {
            arguments.add(ctx.exp().accept(expVisitor));
            if(ctx.single_expr_arg_list() != null) {
                ctx.single_expr_arg_list().accept(this);
            }
            return arguments;
        }
    }
}
package com.multipathp.pprogram;

import com.multipathp.pparser.pBaseVisitor;
import com.multipathp.pparser.pParser;
import com.multipathp.pprogram.ast.PProgram;
import com.multipathp.pprogram.types.*;

public class PTypeConverter extends pBaseVisitor<PType> {
    private PProgram.Builder program;

    public PTypeConverter(PProgram.Builder program) {
        this.program = program;
    }

    @Override
    public PType visitPtype_null(pParser.Ptype_nullContext ctx) {
        return PType.NULL;
    }

    @Override
    public PType visitPtype_int_type(pParser.Ptype_int_typeContext ctx) {
        return PType.INT;
    }

    @Override
    public PType visitPtype_bool(pParser.Ptype_boolContext ctx) {
        return PType.BOOL;
    }

    @Override
    public PType visitPtype_event(pParser.Ptype_eventContext ctx) {
        return PType.EVENT;
    }

    @Override
    public PType visitPtype_machine(pParser.Ptype_machineContext ctx) {
        return PType.MACHINE;
    }

    @Override
    public PType visitPtype_any(pParser.Ptype_anyContext ctx) {
        return PType.ANY;
    }

    @Override
    public PType visitPtype_typedef(pParser.Ptype_typedefContext ctx) {
        PType type = program.getTypedefs().get(ctx.ID().getText());
        if (type == null) {
            ErrorReporter.error("unresolvable typedef", ctx, program.getASTSet());
        }
        return type;
    }

    @Override
    public PType visitPtype_seq(pParser.Ptype_seqContext ctx) {
        return new PTypeSeq(ctx.ptype().accept(this));
    }

    @Override
    public PType visitPtype_map(pParser.Ptype_mapContext ctx) {
        return new PTypeMap(ctx.ptype(0).accept(this), ctx.ptype(1).accept(this));
    }

    @Override
    public PType visitPtype_tuple(pParser.Ptype_tupleContext ctx) {
        return new PTypeTuple(ctx.tup_type_list().accept(new UtilVisitors.TupleTypeListVisitor(this)));
    }


    @Override
    public PType visitPtype_named_tuple(pParser.Ptype_named_tupleContext ctx) {
        return new PTypeNamedTuple(ctx.nmd_tup_type_list().accept(new UtilVisitors.NamedTupleTypeListVisitor(this)));
    }

}
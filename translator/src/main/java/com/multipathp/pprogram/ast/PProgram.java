package com.multipathp.pprogram.ast;

import com.multipathp.pprogram.*;
import com.multipathp.pprogram.types.PType;
import org.inferred.freebuilder.FreeBuilder;

import java.util.*;
import java.util.stream.Collectors;

@FreeBuilder
public abstract class PProgram extends PASTNode {
    /* The original AST set where this program is created from */
    public abstract ParseTreeSetParser.ParseTreeSet getASTSet();

    public abstract Map<String, PEvent> getEventDecls();
    public abstract List<PMachine> getMachines();
    public abstract Map<String, PFunction> getGlobalFunctionDecls();
    public abstract Map<String, PType> getTypedefs();


    @Override
    public int getChildrenCount() {
        return getMachines().size() + getGlobalFunctionDecls().size();
    }

    @Override
    public PASTNode getChild(int i) {
        if(0 <= i && i < getMachines().size()) {
            return getMachines().get(i);
        }
        i -= getMachines().size();
        if(0 <= i && i < getGlobalFunctionDecls().size()) {
            return (PFunction) getGlobalFunctionDecls().values().toArray()[i];
        }
        throw new IndexOutOfBoundsException();
    }

    @Override
    public <T> T accept(PASTVisitor<T> visitor) {
        return visitor.visit(this);
    }

    public static class Builder extends PProgram_Builder {
        @Override
        public PProgram build() {
            PProgram ret = super.build();
            ret.accept(new IdentifierResolver());
            ret.accept(new CallGraphAnnotator());
            return ret;
        }
    }
}
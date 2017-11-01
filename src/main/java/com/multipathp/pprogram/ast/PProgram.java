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

    public abstract PMachine getMainMachine();
    public abstract List<PEvent> getEventDecls();
    public abstract List<PMachine> getMachines();
    public abstract Map<String, PFunction> getGlobalFunctionDecls();
    public abstract Map<String, PType> getTypedefs();

    public Map<PEvent, List<PMachine>> getObserveMap() {
        Map<PEvent, List<PMachine>> ret = new HashMap<>();
        for(PEvent e : getEventDecls()) {
            for(PMachine m : getMachines()) {
                if(m.getObservedEvents().containsKey(e.getName())) {
                    ret.putIfAbsent(e, new ArrayList<>());
                    ret.get(e).add(m);
                }
            }
        }
        return ret;
    }

    public List<PMachine> getObserveMachines() {
        return getMachines().stream().filter(PMachine::isSpec).collect(Collectors.toList());
    }

    public Optional<PEvent> getEventByName(String eventName) {
        return getEventDecls().stream().filter(e -> e.getName().equals(eventName)).findFirst();
    }

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

        public Builder() {
            super();
            addEventDecls(PEvent.EVENT_NULL, PEvent.EVENT_HALT);
        }

        public Builder putGlobalFunctionDecls(PFunction function) {
            return putGlobalFunctionDecls(function.getName(), function);
        }

        @Override
        public PProgram build() {
            getMachines().stream().filter(PMachine::isMain).findFirst().ifPresent(this::setMainMachine);
            PProgram ret = super.build();
            ret.accept(new IdentifierResolver());
            ret.accept(new CallGraphAnnotator());
            return ret;
        }
    }
}
package com.multipathp.pprogram.ast;

/**
 * Created by jianqiaoyang on 6/1/17.
 */
public abstract class PASTVisitor<T> {
    protected T visitChildren(PASTNode node) {
        T result = null;
        for(int i=0; i < node.getChildrenCount(); i++) {
            result = node.getChild(i).accept(this);
        }
        return result;
    }

    public T visit(AnnounceStmt announceStmt) {
        return visitChildren(announceStmt);
    }

    public T visit(ArithmeticExp arithmeticExp) {
        return visitChildren(arithmeticExp);
    }

    public T visit(AssertStmt assertStmt) {
        return visitChildren(assertStmt);
    }

    public T visit(AssignStmt assignStmt) {
        return visitChildren(assignStmt);
    }

    public T visit(BinaryLogicExp binaryLogicExp) {
        return visitChildren(binaryLogicExp);
    }

    public T visit(BoolLiteralExp boolLiteralExp) {
        return visitChildren(boolLiteralExp);
    }

    public T visit(CallExp callExp) {
        return visitChildren(callExp);
    }

    public T visit(CastExp castExp) {
        return visitChildren(castExp);
    }

    public T visit(ComparisonExp comparisonExp) {
        return visitChildren(comparisonExp);
    }

    public T visit(ConditionalStmt conditionalStmt) {
        return visitChildren(conditionalStmt);
    }

    public T visit(DefaultExp defaultExp) {
        return visitChildren(defaultExp);
    }

    public T visit(ExpStmt expStmt) {
        return visitChildren(expStmt);
    }

    public T visit(GetAttributeExp getAttributeExp) {
        return visitChildren(getAttributeExp);
    }

    public T visit(GetIndexExp getIndexExp) {
        return visitChildren(getIndexExp);
    }

    public T visit(GetItemExp getItemExp) {
        return visitChildren(getItemExp);
    }

    public T visit(GroupExp groupExp) {
        return visitChildren(groupExp);
    }

    public T visit(HaltExp haltExp) {
        return visitChildren(haltExp);
    }

    public T visit(IdExp idExp) {
        return visitChildren(idExp);
    }

    public T visit(InExp inExp) {
        return visitChildren(inExp);
    }

    public T visit(InsertStmt insertStmt) {
        return visitChildren(insertStmt);
    }

    public T visit(IntExp intExp) {
        return visitChildren(intExp);
    }

    public T visit(KeysExp keysExp) {
        return visitChildren(keysExp);
    }

    public T visit(MinusExp minusExp) {
        return visitChildren(minusExp);
    }

    public T visit(NamedTupleExp namedTupleExp) {
        return visitChildren(namedTupleExp);
    }

    public T visit(NewExp newExp) {
        return visitChildren(newExp);
    }

    public T visit(NondetExp nondetExp) {
        return visitChildren(nondetExp);
    }

    public T visit(NotExp notExp) {
        return visitChildren(notExp);
    }

    public T visit(NullExp nullExp) {
        return visitChildren(nullExp);
    }

    public T visit(PopStmt popStmt) {
        return visitChildren(popStmt);
    }

    public T visit(PrintStmt printStmt) {
        return visitChildren(printStmt);
    }

    public T visit(RaiseStmt raiseStmt) {
        return visitChildren(raiseStmt);
    }

    public T visit(RemoveStmt removeStmt) {
        return visitChildren(removeStmt);
    }

    public T visit(ReturnStmt returnStmt) {
        return visitChildren(returnStmt);
    }

    public T visit(SendStmt sendStmt) {
        return visitChildren(sendStmt);
    }

    public T visit(SetAttributeStmt setAttributeStmt) {
        return visitChildren(setAttributeStmt);
    }

    public T visit(SetIndexStmt setIndexStmt) {
        return visitChildren(setIndexStmt);
    }

    public T visit(SetItemStmt setItemStmt) {
        return visitChildren(setItemStmt);
    }

    public T visit(SizeofExp sizeofExp) {
        return visitChildren(sizeofExp);
    }

    public T visit(StmtBlock stmtBlock) {
        return visitChildren(stmtBlock);
    }

    public T visit(StmtListStmt stmtListStmt) {
        return visitChildren(stmtListStmt);
    }

    public T visit(ThisExp thisExp) {
        return visitChildren(thisExp);
    }

    public T visit(TupleExp tupleExp) {
        return visitChildren(tupleExp);
    }

    public T visit(ValuesExp valuesExp) {
        return visitChildren(valuesExp);
    }

    public T visit(WhileStmt whileStmt) {
        return visitChildren(whileStmt);
    }

    public T visit(PProgram pProgram) {
        return visitChildren(pProgram);
    }

    public T visit(PFunction pFunction) {
        return visitChildren(pFunction);
    }

    public T visit(PMachine pMachine) {
        return visitChildren(pMachine);
    }

    public T visit(PMachineState pMachineState) {
        return visitChildren(pMachineState);
    }

    public T visit(PTransition pTransition) {
        return visitChildren(pTransition);
    }
}

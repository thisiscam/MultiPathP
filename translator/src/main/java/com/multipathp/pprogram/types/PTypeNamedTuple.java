package com.multipathp.pprogram.types;

import java.util.Collection;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Objects;

public class PTypeNamedTuple extends PType implements IPTypeGetIndexable {
    private LinkedHashMap<String, PType> namedTypes;

    public PTypeNamedTuple(LinkedHashMap<String, PType> namedTypes) {
        super("$namedtuple$", true);
        this.namedTypes.putAll(namedTypes);
    }

    public Collection<PType> getInnerTypes() {
        return namedTypes.values();
    }

    public int getIndexByAttribute(String name) {
        int ret = 0;
        for(Map.Entry<String, PType> entry : namedTypes.entrySet()) {
            if(entry.getKey().equals(name)) {
                return ret;
            }
            ret++;
        }
        return -1;
    }

    public PType getTypeByAttribute(String name) {
        return namedTypes.get(name);
    }

    @Override
    public PType getIndexedInnerType(int index) {
        return (PType) namedTypes.values().toArray()[index];
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (!(o instanceof PTypeNamedTuple)) {
            return false;
        }
        PTypeNamedTuple other = (PTypeNamedTuple) o;
        return Objects.equals(namedTypes, other.namedTypes);
    }

    @Override
    public int hashCode() {
        return Objects.hash(namedTypes, super.hashCode());
    }
}
namespace sylvan {

class Ldd {

public:
    Ldd() { ldd = sylvan_false; lddmc_protect(&ldd); }
    Ldd(const BDD from) : ldd(from) { lddmc_protect(&ldd); }
    Ldd(const Ldd &from) : ldd(from.ldd) { lddmc_protect(&ldd); }
    Ldd(const uint32_t var) { ldd = sylvan_ithvar(var); lddmc_protect(&ldd); }
    ~Ldd() { lddmc_unprotect(&ldd); }

    /**
     * @brief Creates a Ldd representing just the variable index in its positive form
     * The variable index must be a 0<=index<=2^23 (we use 24 bits internally)
     */
    static Ldd lddVar(uint32_t index);

    /**
     * @brief Returns the Ldd representing "True"
     */
    static Ldd lddOne();

    /**
     * @brief Returns the Ldd representing "False"
     */
    static Ldd lddZero();

    /**
     * @brief Returns the Ldd representing a cube of variables, according to the given values.
     * @param variables the variables that will be in the cube in their positive or negative form
     * @param values a character array describing how the variables will appear in the result
     * The length of string must be equal to the number of variables in the cube.
     * For every ith char in string, if it is 0, the corresponding variable will appear in its negative form,
     * if it is 1, it will appear in its positive form, and if it is 2, it will appear as "any", thus it will
     * be skipped.
     */
    static Ldd lddCube(const LddSet &variables, unsigned char *values);

    /**
     * @brief Returns the Ldd representing a cube of variables, according to the given values.
     * @param variables the variables that will be in the cube in their positive or negative form
     * @param string a character array describing how the variables will appear in the result
     * The length of string must be equal to the number of variables in the cube.
     * For every ith char in string, if it is 0, the corresponding variable will appear in its negative form,
     * if it is 1, it will appear in its positive form, and if it is 2, it will appear as "any", thus it will
     * be skipped.
     */
    static Ldd lddCube(const LddSet &variables, std::vector<uint8_t> values);

    int operator==(const Ldd& other) const;
    int operator!=(const Ldd& other) const;
    Ldd& operator=(const Ldd& right);
    int operator<=(const Ldd& other) const;
    int operator>=(const Ldd& other) const;
    int operator<(const Ldd& other) const;
    int operator>(const Ldd& other) const;
    Ldd operator!() const;
    Ldd operator~() const;
    Ldd operator*(const Ldd& other) const;
    Ldd& operator*=(const Ldd& other);
    Ldd operator&(const Ldd& other) const;
    Ldd& operator&=(const Ldd& other);
    Ldd operator+(const Ldd& other) const;
    Ldd& operator+=(const Ldd& other);
    Ldd operator|(const Ldd& other) const;
    Ldd& operator|=(const Ldd& other);
    Ldd operator^(const Ldd& other) const;
    Ldd& operator^=(const Ldd& other);
    Ldd operator-(const Ldd& other) const;
    Ldd& operator-=(const Ldd& other);

    /**
     * @brief Returns non-zero if this Ldd is lddOne() or lddZero()
     */
    int isConstant() const;

    /**
     * @brief Returns non-zero if this Ldd is lddOne() or lddZero()
     */
    int isTerminal() const;

    /**
     * @brief Returns non-zero if this Ldd is lddOne()
     */
    int isOne() const;

    /**
     * @brief Returns non-zero if this Ldd is lddZero()
     */
    int isZero() const;

    /**
     * @brief Returns the top variable index of this Ldd (the variable in the root node)
     */
    uint32_t TopVar() const;

    /**
     * @brief Follows the high edge ("then") of the root node of this Ldd
     */
    Ldd Then() const;

    /**
     * @brief Follows the low edge ("else") of the root node of this Ldd
     */
    Ldd Else() const;

    /**
     * @brief Computes \exists cube: f \and g
     */
    Ldd AndAbstract(const Ldd& g, const LddSet& cube) const;

    /**
     * @brief Computes \exists cube: f
     */
    Ldd ExistAbstract(const LddSet& cube) const;

    /**
     * @brief Computes \forall cube: f
     */
    Ldd UnivAbstract(const LddSet& cube) const;

    /**
     * @brief Computes if f then g else h
     */
    Ldd Ite(const Ldd& g, const Ldd& h) const;

    /**
     * @brief Computes f \and g
     */
    Ldd And(const Ldd& g) const;

    /**
     * @brief Computes f \or g
     */
    Ldd Or(const Ldd& g) const;

    /**
     * @brief Computes \not (f \and g)
     */
    Ldd Nand(const Ldd& g) const;

    /**
     * @brief Computes \not (f \or g)
     */
    Ldd Nor(const Ldd& g) const;

    /**
     * @brief Computes f \xor g
     */
    Ldd Xor(const Ldd& g) const;

    /**
     * @brief Computes \not (f \xor g), i.e. f \equiv g
     */
    Ldd Xnor(const Ldd& g) const;

    /**
     * @brief Returns whether all elements in f are also in g
     */
    int Leq(const Ldd& g) const;

    /**
     * @brief Computes the reverse application of a transition relation to this set.
     * @param relation the transition relation to apply
     * @param cube the variables that are in the transition relation
     * This function assumes that s,t are interleaved with s even and t odd (s+1).
     * Other variables in the relation are ignored (by existential quantification)
     * Set cube to "false" (illegal cube) to assume all encountered variables are in s,t
     *
     * Use this function to concatenate two relations   --> -->
     * or to take the 'previous' of a set               -->  S
     */
    Ldd RelPrev(const Ldd& relation, const LddSet& cube) const;

    /**
     * @brief Computes the application of a transition relation to this set.
     * @param relation the transition relation to apply
     * @param cube the variables that are in the transition relation
     * This function assumes that s,t are interleaved with s even and t odd (s+1).
     * Other variables in the relation are ignored (by existential quantification)
     * Set cube to "false" (illegal cube) to assume all encountered variables are in s,t
     *
     * Use this function to take the 'next' of a set     S  -->
     */
    Ldd RelNext(const Ldd& relation, const LddSet& cube) const;

    /**
     * @brief Computes the transitive closure by traversing the BDD recursively.
     * See Y. Matsunaga, P. C. McGeer, R. K. Brayton
     *     On Computing the Transitive Closre of a State Transition Relation
     *     30th ACM Design Automation Conference, 1993.
     */
    Ldd Closure() const;

    /**
     * @brief Computes the constrain f @ c
     */
    Ldd Constrain(const Ldd &c) const;

    /**
     * @brief Computes the BDD restrict according to Coudert and Madre's algorithm (ICCAD90).
     */
    Ldd Restrict(const Ldd &c) const;

    /**
     * @brief Functional composition. Whenever a variable v in the map m is found in the BDD,
     *        it is substituted by the associated function.
     * You can also use this function to implement variable reordering.
     */
    Ldd Compose(const LddMap &m) const;

    /**
     * @brief Substitute all variables in the array from by the corresponding variables in to.
     */
    Ldd Permute(const std::vector<uint32_t>& from, const std::vector<uint32_t>& to) const;

    /**
     * @brief Computes the support of a Ldd.
     */
    Ldd Support() const;

    /**
     * @brief Gets the BDD of this Ldd (for C functions)
     */
    BDD GetBDD() const;

    /**
     * @brief Writes .dot file of this Ldd. Not thread-safe!
     */
    void PrintDot(FILE *out) const;

    /**
     * @brief Gets a SHA2 hash that describes the structure of this Ldd.
     * @param string a character array of at least 65 characters (includes zero-termination)
     * This hash is 64 characters long and is independent of the memory locations of BDD nodes.
     */
    void GetShaHash(char *string) const;

    std::string GetShaHash() const;

    /**
     * @brief Computes the number of satisfying variable assignments, using variables in cube.
     */
    double SatCount(const LddSet &cube) const;

    /**
     * @brief Compute the number of satisfying variable assignments, using the given number of variables.
     */
    double SatCount(const size_t nvars) const;

    /**
     * @brief Gets one satisfying assignment according to the variables.
     * @param variables The set of variables to be assigned, must include the support of the Ldd.
     */
    void PickOneCube(const LddSet &variables, uint8_t *string) const;

    /**
     * @brief Gets one satisfying assignment according to the variables.
     * @param variables The set of variables to be assigned, must include the support of the Ldd.
     * Returns an empty vector when either this Ldd equals lddZero() or the cube is empty.
     */
    std::vector<bool> PickOneCube(const LddSet &variables) const;

    /**
     * @brief Gets a cube that satisfies this Ldd.
     */
    Ldd PickOneCube() const;

    /**
     * @brief Faster version of: *this + Sylvan::lddCube(variables, values);
     */
    Ldd UnionCube(const LddSet &variables, uint8_t *values) const;

    /**
     * @brief Faster version of: *this + Sylvan::lddCube(variables, values);
     */
    Ldd UnionCube(const LddSet &variables, std::vector<uint8_t> values) const;

    /**
     * @brief Generate a cube representing a set of variables
     */
    static Ldd VectorCube(const std::vector<Ldd> variables);

    /**
     * @brief Generate a cube representing a set of variables
     * @param variables An sorted set of variable indices
     */
    static Ldd VariablesCube(const std::vector<uint32_t> variables);

    /**
     * @brief Gets the number of nodes in this Ldd. Not thread-safe!
     */
    size_t NodeCount() const;

private:
    BDD ldd;
};

}
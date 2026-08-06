#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "../common/adapter.h"

#include "SROBDD/bindings/cpp.h"



class gpudecide_bdd_adapter
{
	public:
	static constexpr std::string_view name = "GPUdecide";
	static constexpr std::string_view dd   = "BDD";

	using dd_t         = gpudecide::node_ref;
	using build_node_t = gpudecide::node_ref;
	
	private:

	gpudecide::bdd _bdd;
	gpudecide::node_ref _latest_build;

	// Init and Deinit
	public:
	gpudecide_bdd_adapter(uint32_t varcount) : _bdd(varcount)
	{}

	int
	run(std::function<int()> f)
	{
		return f();
	}

	// BDD Operations
	public:
	inline gpudecide::node_ref
	top()
	{
		return _bdd.logical_true();
	}

	inline gpudecide::node_ref
	bot()
	{
		return _bdd.logical_false();
	}

	inline gpudecide::node_ref
	ithvar(uint32_t label)
	{
		return _bdd.get_variable(label);
	}

	inline gpudecide::node_ref
	nithvar(uint32_t label)
	{
		gpudecide::node_ref v = ithvar(label);
		return _bdd.logical_not(v);
	}

	// template <typename IT>
	// inline oxidd::bdd_function
	// cube(IT rbegin, IT rend)
	// {
	// 	oxidd::bdd_function cube = top();
	// 	while (rbegin != rend) { cube &= _manager.var(*(rbegin++)); }
	// 	return cube;
	// }

	// inline oxidd::bdd_function
	// cube(const std::function<bool(int)>& pred)
	// {
	// 	oxidd::bdd_function cube = top();
	// 	const oxidd::var_no_t num_vars = _manager.num_vars();
	// 	for (oxidd::var_no_t i = 0; i < num_vars; ++i) {
	// 	if (pred(i)) cube &= _manager.var(i);
	// 	}
	// 	return cube;
	// }

	// inline oxidd::bdd_function
	// apply_and(const oxidd::bdd_function& f, const oxidd::bdd_function& g)
	// {
	// 	return f & g;
	// }

	// inline oxidd::bdd_function
	// apply_or(const oxidd::bdd_function& f, const oxidd::bdd_function& g)
	// {
	// 	return f | g;
	// }

	inline gpudecide::node_ref
	apply_diff(const gpudecide::node_ref& f, const gpudecide::node_ref& g)
	{
		return _bdd.logical_difference(f, g);
	}

	inline gpudecide::node_ref
	apply_imp(const gpudecide::node_ref& f, const gpudecide::node_ref& g)
	{
		return _bdd.logical_implication(f, g);
	}

	// inline oxidd::bdd_function
	// apply_xor(const oxidd::bdd_function& f, const oxidd::bdd_function& g)
	// {
	// 	return f ^ g;
	// }

	// inline oxidd::bdd_function
	// apply_xnor(const oxidd::bdd_function& f, const oxidd::bdd_function& g)
	// {
	// 	return f.equiv(g);
	// }

	inline gpudecide::node_ref
	ite(gpudecide::node_ref i, gpudecide::node_ref t, gpudecide::node_ref e)
	{
		return _bdd.logical_ite(i, t, e);
	}

	// template <typename IT>
	// inline oxidd::bdd_function
	// extend(const oxidd::bdd_function& f, IT /*begin*/, IT /*end*/)
	// {
	// 	return f;
	// }

	// template <typename IT>
	// inline oxidd::bdd_function
	// extend(const oxidd::bdd_function& f, IT /*begin*/, IT /*end*/)
	// {
	// return f;
	// }

	inline gpudecide::node_ref
	exists(const gpudecide::node_ref& b, int label)
	{
		return _bdd.exists(b, label);
	}

	// inline oxidd::bdd_function
	// exists(const oxidd::bdd_function& b, const std::function<bool(int)>& pred)
	// {
	// 	std::vector<uint32_t> quantification;
	// 	for (int i = 0; i < _bdd)
	// 	return _bdd.exists(b, quantification);
	// }

	// template <typename IT>
	// inline oxidd::bdd_function
	// exists(const oxidd::bdd_function& b, IT rbegin, IT rend)
	// {
	// return b.exists(cube(rbegin, rend));
	// }

	inline gpudecide::node_ref
	forall(const gpudecide::node_ref& b, int label)
	{
		return _bdd.for_all(b, label);
	}

	// inline oxidd::bdd_function
	// forall(const oxidd::bdd_function& b, const std::function<bool(int)>& pred)
	// {
	// return b.forall(cube(pred));
	// }

	// template <typename IT>
	// inline oxidd::bdd_function
	// forall(const oxidd::bdd_function& b, IT rbegin, IT rend)
	// {
	// return b.forall(rbegin, rend);
	// }

	// inline oxidd::bdd_function
	// relnext(const oxidd::bdd_function& states,
	// 		const oxidd::bdd_function& rel,
	// 		const oxidd::bdd_function& /*rel_support*/)
	// {
	// if (_relnext_vars == bot()) {
	// 	_relnext_vars = cube([](int x) { return x % 2 == 0; });

	// 	const oxidd::var_no_t num_vars = _manager.num_vars();
	// 	assert(num_vars % 2 == 0);
	// 	_relnext_pairs = oxidd::bdd_substitution(
	// 	std::views::iota(oxidd::var_no_t(0), num_vars / 2)
	// 	| std::views::transform(
	// 		[this](oxidd::var_no_t v) -> std::pair<oxidd::var_no_t, oxidd::bdd_function> {
	// 		return { 2 * v + 1, _manager.var(2 * v) };
	// 		}));
	// }

	// return states.apply_exists(oxidd::util::boolean_operator::AND, rel, _relnext_vars)
	// 	.substitute(_relnext_pairs);
	// }

	// inline oxidd::bdd_function
	// relprev(const oxidd::bdd_function& states,
	// 		const oxidd::bdd_function& rel,
	// 		const oxidd::bdd_function& /*rel_support*/)
	// {
	// if (_relprev_vars == bot()) {
	// 	_relprev_vars = cube([](int x) { return x % 2 == 1; });

	// 	const oxidd::var_no_t num_vars = _manager.num_vars();
	// 	assert(num_vars % 2 == 0);
	// 	_relprev_pairs = oxidd::bdd_substitution(
	// 	std::views::iota(oxidd::var_no_t(0), num_vars / 2)
	// 	| std::views::transform(
	// 		[this](oxidd::var_no_t v) -> std::pair<oxidd::var_no_t, oxidd::bdd_function> {
	// 		return { 2 * v, _manager.var(2 * v + 1) };
	// 		}));
	// }

	// return states.substitute(_relprev_pairs)
	// 	.apply_exists(oxidd::util::boolean_operator::AND, rel, _relprev_vars);
	// }

	inline uint64_t
	nodecount(const gpudecide::node_ref f)
	{
		return _bdd.count_irreducible_nodes();
	}

	inline uint64_t
	satcount(gpudecide::node_ref ref)
	{
		auto results = _bdd.count_satisfying_assignments();
		return ref.get_metric(results);
	}

	// inline uint64_t
	// satcount(const oxidd::bdd_function& f, const size_t vc)
	// {
	// assert(vc <= _manager.num_vars());
	// return f.sat_count_double(vc);
	// }
	
	// inline oxidd::bdd_function
	// satone(const oxidd::bdd_function& f)
	// {
	// return f.pick_cube_dd();
	// }

	// inline oxidd::bdd_function
	// satone(const oxidd::bdd_function& f, const oxidd::bdd_function& c)
	// {
	// return f.pick_cube_dd_set(c);
	// }

	// inline std::vector<std::pair<uint32_t, char>>
	// pickcube(const oxidd::bdd_function& f)
	// {
	// oxidd::util::assignment sat = f.pick_cube();

	// std::vector<std::pair<uint32_t, char>> res;
	// res.reserve(sat.size());
	// for (uint32_t x = 0; x < sat.size(); ++x) {
	// 	const oxidd::util::opt_bool val = sat[x];
	// 	if (val == oxidd::util::opt_bool::NONE) continue;

	// 	res.emplace_back(x, '0' + static_cast<char>(val));
	// }

	// return res;
	// }

	// void
	// print_dot(const oxidd::bdd_function&, const std::string&)
	// {
	// std::cerr << "'oxidd_bdd_adapter' does not yet support dot export" << std::endl;
	// }

	// void
	// save(const oxidd::bdd_function&, const std::string&)
	// {
	// std::cerr << "'oxidd_bdd_adapter' does not yet support BDD export" << std::endl;
	// }

	// BDD Build Operations
	public:
	inline gpudecide::node_ref
	build_node(const bool value)
	{
		const gpudecide::node_ref res = value ? top() : bot();
		if (_latest_build.is_invalid() || _latest_build == top() || _latest_build == bot()) {
			_latest_build = res;
		}
		return res;
	}

	inline gpudecide::node_ref
	build_node(const uint32_t label, const gpudecide::node_ref& low, const gpudecide::node_ref& high)
	{
		_latest_build = ite(ithvar(label), high, low);
		return _latest_build;
	}

	inline gpudecide::node_ref
	build()
	{
		return std::move(_latest_build);
	}

	// // Statistics
	// public:
	// inline size_t
	// allocated_nodes()
	// {
	// return _manager.num_inner_nodes();
	// }

	// void
	// print_stats()
	// {
	// std::cout << "OxiDD statistics:" << std::endl
	// 			<< "  inner nodes stored in manager: " << _manager.num_inner_nodes() << std::endl;
	// oxidd::capi::oxidd_bdd_print_stats();
	// }
};

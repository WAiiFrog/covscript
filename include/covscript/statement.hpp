#pragma once
/*
* Covariant Script Statement
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Copyright (C) 2018 Michael Lee(李登淳)
* Email: mikecovlee@163.com
* Github: https://github.com/mikecovlee
*/
#include <covscript/instance.hpp>

namespace cs {
	class statement_expression final : public statement_base {
		cov::tree<token_base *> mTree;
	public:
		statement_expression() = delete;

		statement_expression(const cov::tree<token_base *> &tree, context_t c, token_base *ptr) : statement_base(c,
			        ptr),
			mTree(tree) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::expression_;
		}

		virtual void run() override;

		virtual void repl_run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_involve final : public statement_base {
		bool mOverride = false;
		cov::tree<token_base *> mTree;
	public:
		statement_involve() = delete;

		statement_involve(const cov::tree<token_base *> &tree, bool is_override, context_t c, token_base *ptr)
			: statement_base(c, ptr), mOverride(is_override), mTree(tree) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::involve_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_var final : public statement_base {
		instance_type::define_var_profile mDvp;
	public:
		statement_var() = delete;

		statement_var(const instance_type::define_var_profile &dvp, context_t c, token_base *ptr) : statement_base(c,
			        ptr),
			mDvp(dvp) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::var_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_constant final : public statement_base {
		std::string mName;
		var mVal;
	public:
		statement_constant() = delete;

		statement_constant(const std::string &name, const var &val, context_t c, token_base *ptr) : statement_base(c,
			        ptr),
			mName(name),
			mVal(val) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::var_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_break final : public statement_base {
	public:
		statement_break() = default;

		statement_break(context_t c, token_base *ptr) : statement_base(c, ptr) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::break_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_continue final : public statement_base {
	public:
		statement_continue() = default;

		statement_continue(context_t c, token_base *ptr) : statement_base(c, ptr) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::continue_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_block final : public statement_base {
		std::deque<statement_base *> mBlock;
	public:
		statement_block() = delete;

		statement_block(const std::deque<statement_base *> &block, context_t c, token_base *ptr) : statement_base(c,
			        ptr),
			mBlock(block) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::block_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;

		const std::deque<statement_base *>& get_block() const
		{
			return this->mBlock;
		}
	};

	class statement_namespace final : public statement_base {
		std::string mName;
		std::deque<statement_base *> mBlock;
	public:
		statement_namespace() = delete;

		statement_namespace(token_base *tbp, const std::deque<statement_base *> &block, context_t c, token_base *ptr)
			: statement_base(c, ptr), mName(static_cast<token_id *>(tbp)->get_id()), mBlock(block) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::namespace_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_if final : public statement_base {
		cov::tree<token_base *> mTree;
		std::deque<statement_base *> mBlock;
	public:
		statement_if() = delete;

		statement_if(const cov::tree<token_base *> &tree, const std::deque<statement_base *> &block, context_t c,
		             token_base *ptr) : statement_base(c, ptr), mTree(tree), mBlock(block) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::if_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_ifelse final : public statement_base {
		cov::tree<token_base *> mTree;
		std::deque<statement_base *> mBlock;
		std::deque<statement_base *> mElseBlock;
	public:
		statement_ifelse() = delete;

		statement_ifelse(const cov::tree<token_base *> &tree, const std::deque<statement_base *> &btrue,
		                 const std::deque<statement_base *> &bfalse, context_t c, token_base *ptr) : statement_base(c,
			                         ptr),
			mTree(tree),
			mBlock(btrue),
			mElseBlock(
			    bfalse) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::if_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_else final : public statement_base {
	public:
		statement_else() = default;

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::else_;
		}

		virtual void run() override
		{
			throw runtime_error("Do not allowed standalone else statement.");
		}

		virtual void dump(std::ostream&) const override
		{
			throw runtime_error("Do not allowed standalone else statement.");
		}
	};

	class statement_switch final : public statement_base {
		cov::tree<token_base *> mTree;
		statement_block *mDefault = nullptr;
		map_t<var, statement_block *> mCases;
	public:
		statement_switch() = delete;

		statement_switch(const cov::tree<token_base *> &tree, const map_t<var, statement_block *> &cases,
		                 statement_block *dptr, context_t c, token_base *ptr) : statement_base(c, ptr), mTree(tree),
			mDefault(dptr), mCases(cases) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::switch_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_case final : public statement_base {
		var mTag;
		statement_block *mBlock;
	public:
		statement_case() = delete;

		statement_case(const var &tag, const std::deque<statement_base *> &b, context_t c, token_base *ptr)
			: statement_base(c, ptr), mTag(copy(tag)), mBlock(new statement_block(b, c, ptr)) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::case_;
		}

		virtual void run() override
		{
			throw runtime_error("Do not allowed standalone case statement.");
		}

		virtual void dump(std::ostream&) const override
		{
			throw runtime_error("Do not allowed standalone case statement.");
		}

		const var &get_tag() const
		{
			return this->mTag;
		}

		statement_block *get_block() const
		{
			return this->mBlock;
		}
	};

	class statement_default final : public statement_base {
		statement_block *mBlock;
	public:
		statement_default() = delete;

		statement_default(const std::deque<statement_base *> &b, context_t c, token_base *ptr) : statement_base(c, ptr),
			mBlock(new statement_block(
			           b, c, ptr)) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::default_;
		}

		virtual void run() override
		{
			throw runtime_error("Do not allowed standalone default statement.");
		}

		virtual void dump(std::ostream&) const override
		{
			throw runtime_error("Do not allowed standalone default statement.");
		}

		statement_block *get_block() const
		{
			return this->mBlock;
		}
	};

	class statement_while final : public statement_base {
		cov::tree<token_base *> mTree;
		std::deque<statement_base *> mBlock;
	public:
		statement_while() = delete;

		statement_while(const cov::tree<token_base *> &tree, const std::deque<statement_base *> &b, context_t c,
		                token_base *ptr) : statement_base(c, ptr), mTree(tree), mBlock(b) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::while_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_until final : public statement_base {
		token_expr *mExpr = nullptr;
	public:
		statement_until() = delete;

		statement_until(token_expr *expr, context_t c, token_base *ptr) : statement_base(c, ptr), mExpr(expr) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::until_;
		}

		token_expr *get_expr() const
		{
			return mExpr;
		}

		virtual void run() override
		{
			throw runtime_error("Do not allowed standalone until statement.");
		}

		virtual void dump(std::ostream&) const override
		{
			throw runtime_error("Do not allowed standalone until statement.");
		}
	};

	class statement_loop final : public statement_base {
		token_expr *mExpr = nullptr;
		std::deque<statement_base *> mBlock;
	public:
		statement_loop() = delete;

		statement_loop(token_expr *expr, const std::deque<statement_base *> &b, context_t c, token_base *ptr)
			: statement_base(c, ptr), mExpr(expr), mBlock(b) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::loop_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_for final : public statement_base {
		instance_type::define_var_profile mDvp;
		cov::tree<token_base *> mEnd;
		cov::tree<token_base *> mStep;
		std::deque<statement_base *> mBlock;
	public:
		statement_for() = delete;

		statement_for(cov::tree<token_base *> &tree0, const cov::tree<token_base *> &tree1,
		              const cov::tree<token_base *> &tree2, const std::deque<statement_base *> &b, context_t c,
		              token_base *ptr) : statement_base(c, ptr), mEnd(tree1), mStep(tree2), mBlock(b)
		{
			context->instance->parse_define_var(tree0, mDvp);
		}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::for_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_foreach final : public statement_base {
		std::string mIt;
		cov::tree<token_base *> mObj;
		std::deque<statement_base *> mBlock;
	public:
		statement_foreach() = delete;

		statement_foreach(const std::string &it, const cov::tree<token_base *> &tree1,
		                  const std::deque<statement_base *> &b, context_t c, token_base *ptr) : statement_base(c, ptr),
			mIt(it), mObj(tree1),
			mBlock(b) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::foreach_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_struct final : public statement_base {
		std::string mName;
		struct_builder mBuilder;
		// Debug Information
		cov::tree<token_base *> mParent;
		std::deque<statement_base *> mBlock;
	public:
		statement_struct() = delete;

		statement_struct(const std::string &name, const cov::tree<token_base *> &parent, const std::deque<statement_base *> &method, context_t c, token_base *ptr) : statement_base(c, ptr), mName(name), mBuilder(c, name, parent, method), mParent(parent), mBlock(method) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::struct_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_function final : public statement_base {
		std::string mName;
		function mFunc;
		bool mOverride = false;
		bool mIsMemFn = false;
		// Debug Information
		std::vector<std::string> mArgs;
		std::deque<statement_base *> mBlock;
	public:
		statement_function() = delete;

		statement_function(const std::string &name, const std::vector<std::string> &args, const std::deque<statement_base *> &body, bool is_override, context_t c, token_base *ptr) : statement_base(c, ptr), mName(name), mFunc(c, args, body), mOverride(is_override), mArgs(args), mBlock(body) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::function_;
		}

		void set_mem_fn()
		{
			mFunc.add_this();
			mIsMemFn = true;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_return final : public statement_base {
		cov::tree<token_base *> mTree;
	public:
		statement_return() = delete;

		statement_return(const cov::tree<token_base *> &tree, context_t c, token_base *ptr) : statement_base(c, ptr),
			mTree(tree) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::return_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_end final : public statement_base {
	public:
		statement_end() = default;

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::end_;
		}

		virtual void run() override
		{
			throw runtime_error("Do not allowed standalone end statement.");
		}

		virtual void dump(std::ostream&) const override
		{
			throw runtime_error("Do not allowed standalone end statement.");
		}
	};

	class statement_try final : public statement_base {
		std::string mName;
		std::deque<statement_base *> mTryBody;
		std::deque<statement_base *> mCatchBody;
	public:
		statement_try() = delete;

		statement_try(const std::string &name, const std::deque<statement_base *> &tbody,
		              const std::deque<statement_base *> &cbody, context_t c, token_base *ptr) : statement_base(c, ptr),
			mName(name),
			mTryBody(tbody),
			mCatchBody(cbody) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::try_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};

	class statement_catch final : public statement_base {
		std::string mName;
	public:
		statement_catch() = delete;

		statement_catch(const std::string &name, context_t c, token_base *ptr) : statement_base(c, ptr), mName(name) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::catch_;
		}

		const std::string &get_name() const
		{
			return this->mName;
		}

		virtual void run() override
		{
			throw runtime_error("Do not allowed standalone catch statement.");
		}

		virtual void dump(std::ostream&) const override
		{
			throw runtime_error("Do not allowed standalone catch statement.");
		}
	};

	class statement_throw final : public statement_base {
		cov::tree<token_base *> mTree;
	public:
		statement_throw() = delete;

		statement_throw(const cov::tree<token_base *> &tree, context_t c, token_base *ptr) : statement_base(c, ptr),
			mTree(tree) {}

		virtual statement_types get_type() const noexcept override
		{
			return statement_types::throw_;
		}

		virtual void run() override;

		virtual void dump(std::ostream&) const override;
	};
}

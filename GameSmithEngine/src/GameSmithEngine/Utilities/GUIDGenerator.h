#pragma once
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {

	struct idData {
		unsigned int ID1;
		unsigned short ID2;
		unsigned short ID3;
		unsigned long long ID4;
	};

	class GE_API ID {
	public:
		ID() : m_Data() {}
		ID(unsigned long ID1, unsigned short ID2, unsigned short ID3, unsigned long long ID4) : m_Data({ID1, ID2, ID3, ID4}) {}
		ID(idData externalData) : m_Data(externalData) {}
		void operator=(ID& other) { m_Data = other.m_Data; }
		void operator=(const ID& other) { m_Data = other.m_Data; }

		inline bool operator==(const ID& rhs) const {
			return !(
				m_Data.ID1 != rhs.m_Data.ID1 ||
				m_Data.ID2 != rhs.m_Data.ID2 ||
				m_Data.ID3 != rhs.m_Data.ID3 ||
				m_Data.ID4 != rhs.m_Data.ID4
				);
		}

		idData getData() const { return m_Data; }
	private:
		idData m_Data;
	};

	class GE_API GUIDGenerator
	{
	public:
		static ID GenerateID();
	};

	class GE_API IDObjectInterface {
	public:
		virtual ~IDObjectInterface() = default;
		virtual const ID& GetID() const = 0;
		virtual void SetID(ID& newID) = 0;
	
	};

	class GE_API IDObject : public virtual IDObjectInterface {
	public:
		IDObject() : m_ID(GUIDGenerator::GenerateID()) {}
		virtual ~IDObject() = default;
		inline const ID& GetID() const override { return m_ID; }
		inline void SetID(ID& newID) override { m_ID = newID; }
	private:
		ID m_ID;
	};
};

struct IDHasher {
	inline std::size_t operator()(const GameSmith::ID& key) const {
		std::size_t hash = 0;

		GameSmith::idData data = key.getData();

		hash |= (std::size_t)data.ID1 | (std::size_t)data.ID2 | (std::size_t)data.ID3 | (std::size_t)data.ID4;

		return hash;
	}
};


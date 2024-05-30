#pragma once

namespace GameSmith {

	struct idData {
		unsigned int ID1;
		unsigned short ID2;
		unsigned short ID3;
		unsigned long long ID4;
	};

	class ID {
	public:
		ID() : m_Data() {}
		ID(unsigned long ID1, unsigned short ID2, unsigned short ID3, unsigned long long ID4) : m_Data({ID1, ID2, ID3, ID4}) {}
		ID(idData externalData) : m_Data(externalData) {}
		//ID operator=(ID& other) { return ID(other.m_Data.ID1, other.m_Data.ID2, m_Data.ID3, other.m_Data.ID4); }
		void operator=(ID& other) { m_Data = other.m_Data; }

		inline std::size_t operator()(const ID& key) const {
			std::size_t hash = 0;

			hash |= (std::size_t)m_Data.ID1 | (std::size_t)m_Data.ID2 | (std::size_t)m_Data.ID3 | (std::size_t)m_Data.ID4;

			return hash;
		}

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

	

	class GUIDGenerator
	{
	public:
		static ID GenerateID();
	};

};


DAT
======

## Header

| Offset (bytes) | Size (bytes) | Type                   | Description                                                                                              |
| -------------- | ------------ |------------------------|----------------------------------------------------------------------------------------------------------|
| 0              | 4            | std::uint32_t          | Number of sections (m_count)</br>Typically 11 or 2.</br>If m_count == 2 then only Section 7 and 8 exist. |
| 4              | 4 * m_count  | std::vector<uint32_t>  | Offsets of each section (m_offsets)                                                                      |


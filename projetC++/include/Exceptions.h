#pragma once

#include <stdexcept>
#include <string>

/*!
 * \brief Base exception class for blockchain errors.
 */
class BlockchainException : public std::runtime_error
{
public:
    explicit BlockchainException(const std::string &message)
        : std::runtime_error(message) {}
};

/*!
 * \brief Thrown when validation fails (e.g., block invalid, signature invalid).
 */
class ValidationException : public BlockchainException
{
public:
    explicit ValidationException(const std::string &message)
        : BlockchainException(message) {}
};

/*!
 * \brief Thrown when file I/O operations fail or data is corrupted.
 */
class FileOperationException : public BlockchainException
{
public:
    explicit FileOperationException(const std::string &message)
        : BlockchainException(message) {}
};

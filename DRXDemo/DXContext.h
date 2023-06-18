#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include <d3dx12.h>
#include <directxmath.h>

#include <memory>
#include <vector>

#include "Window.h"
#include "CommandQueue.h"

namespace DRXDemo
{
    class DXContext
    {
    public:
        DXContext(const Window& window, uint32_t numBuffers);
        DXContext(const DXContext&) = delete;
        DXContext(DXContext&&) noexcept = delete;
        DXContext& operator=(const DXContext& rhs) = delete;
        DXContext& operator=(DXContext&&) noexcept = delete;
        ~DXContext();

        Microsoft::WRL::ComPtr<IDXGIAdapter4> Adapter;
        Microsoft::WRL::ComPtr<ID3D12Device5> Device;
        std::unique_ptr<CommandQueue> DirectCommandQueue;
        std::unique_ptr<CommandQueue> CopyCommandQueue;

        void SetVSync(bool enabled);
        bool IsVSyncEnabled() const;

        void SetRaytracing(bool enabled);
        bool IsRaytracingEnabled() const;

        void Flush();
        UINT GetNumberBuffers() const;
        UINT GetCurrentBackBufferIndex() const;
        Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentBackBuffer() const;
        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRenderTargetView() const;
        UINT Present();
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
            Microsoft::WRL::ComPtr<ID3D12Device2> device,
            D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);
        UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type) const;

        bool IsRaytracingSupported();

    private:

        bool _useWarp = false;
        uint32_t _numBuffers;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
        UINT _currentBackBufferIndex;
        std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> _backBuffers;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvDescriptorHeap;
        UINT _rtvDescriptorSize;

        bool _vSyncEnabled = true;
        bool _raytracingEnabled = false;
        bool _tearingSupported = false;

        void _EnableDebugLayer();
        
        Microsoft::WRL::ComPtr<IDXGIAdapter4> _GetAdapter(bool useWarp);
        
        Microsoft::WRL::ComPtr<ID3D12Device5> _CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);
        
        bool _CheckTearingSupport();
        
        Microsoft::WRL::ComPtr<IDXGISwapChain4> _CreateSwapChain(
            HWND hWnd,
            Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue,
            uint32_t width,
            uint32_t height,
            uint32_t bufferCount);

        void _UpdateRenderTargetViews();
    };
}
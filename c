local Players = game:GetService("Players")
local UserInputService = game:GetService("UserInputService")

-- Safe initialization
local function waitForLocalPlayer()
    while not Players.LocalPlayer do
        task.wait(1)
    end
    return Players.LocalPlayer
end

local localPlayer = waitForLocalPlayer()
local GUI_CHECK_INTERVAL = 2

-- Enhanced button finder
local function findAcceptButton()
    local gui = localPlayer:WaitForChild("PlayerGui", 10) -- Timeout 10 detik
    if not gui then return nil end

    -- Cari berdasarkan kriteria bertahap
    local searchPatterns = {
        {Class = "TextButton", Text = "Accept"},
        {Class = "ImageButton", Name = "AcceptBtn"},
        {Class = "TextButton", Name = "BtnAccept"}
    }

    for _, pattern in ipairs(searchPatterns) do
        for _, element in ipairs(gui:GetDescendants()) do
            if element:IsA(pattern.Class) then
                if pattern.Text and string.match(element.Text:lower(), "accept") then
                    return element
                elseif pattern.Name and string.match(element.Name:lower(), "accept") then
                    return element
                end
            end
        end
    end
    return nil
end

-- Robust click simulation
local function clickAcceptButton(button)
    local success, err = pcall(function()
        -- Coba semua metode klik yang mungkin
        if button:IsA("TextButton") or button:IsA("ImageButton") then
            -- Metode 1: Activated event
            button.Activated:Fire()
            
            -- Metode 2: Mouse click simulation
            if button:FindFirstChild("MouseButton1Click") then
                button.MouseButton1Click:Fire()
            end
            
            -- Metode 3: Direct callback (untuk beberapa game)
            if button:FindFirstChild("OnClick") then
                button.OnClick:Fire()
            end
            
            return true
        end
        return false
    end)
    
    if not success then
        warn("Gagal mengklik tombol:", err)
    end
    return success
end

-- Main execution flow
local function main()
    while true do
        local acceptButton = findAcceptButton()
        
        if acceptButton then
            print("Tombol Accept ditemukan di:", acceptButton:GetFullName())
            if clickAcceptButton(acceptButton) then
                print("Berhasil mengklik Accept!")
            end
        else
            print("Memindai gift...")
        end
        
        task.wait(GUI_CHECK_INTERVAL)
    end
end

-- Error handling wrapper
local success, err = pcall(main)
if not success then
    warn("Error utama:", err)
end

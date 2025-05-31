local Players = game:GetService("Players")
local UserInputService = game:GetService("UserInputService")
local RunService = game:GetService("RunService")

local localPlayer = Players.LocalPlayer
while not localPlayer do
    task.wait(1)
    localPlayer = Players.LocalPlayer
end

-- ===============================================
-- FUNGSI UTAMA UNTUK MENDAPATKAN TOMBOL ACCEPT
-- ===============================================
local function getAcceptButton()
    -- Path spesifik berdasarkan error sebelumnya
    local path = {
        "PlayerGui",
        "FriendInvites",
        "Insertio!Point",
        "FriendInviteTemplate",
        "Holder",
        "Frame",
        "Accept"
    }
    
    -- Cari berdasarkan path
    local current = localPlayer
    for _, childName in ipairs(path) do
        current = current:FindFirstChild(childName)
        if not current then return nil end
    end
    return current
end

-- ===============================================
-- 3 METODE KLIK YANG BERBEDA
-- ===============================================
local function clickButton(button)
    -- Metode 1: Via callback langsung
    if button:FindFirstChild("OnClick") then
        button.OnClick:Fire()
        return true
    end
    
    -- Metode 2: Via event Activated
    if button:IsA("TextButton") or button:IsA("ImageButton") then
        for _, event in ipairs(getconnections(button.Activated)) do
            event:Fire()
        end
        return true
    end
    
    -- Metode 3: Simulasi klik mouse (paling universal)
    local absPos = button.AbsolutePosition
    local absSize = button.AbsoluteSize
    local clickPos = Vector2.new(
        absPos.X + absSize.X/2,
        absPos.Y + absSize.Y/2
    )
    
    UserInputService:SendMouseButtonEvent(
        clickPos.X,
        clickPos.Y,
        0, -- MouseButton1
        true, -- Press
        game
    )
    task.wait(0.05)
    UserInputService:SendMouseButtonEvent(
        clickPos.X,
        clickPos.Y,
        0, -- MouseButton1
        false, -- Release
        game
    )
    
    return true
end

-- ===============================================
-- SISTEM PEMINDAIAN REAL-TIME
-- ===============================================
local connection
connection = RunService.Heartbeat:Connect(function()
    local button = getAcceptButton()
    if button then
        if clickButton(button) then
            print("[SUCCESS] Gift diterima!")
            connection:Disconnect() -- Berhenti setelah berhasil
        end
    end
end)

print("System gift acceptor aktif!")
